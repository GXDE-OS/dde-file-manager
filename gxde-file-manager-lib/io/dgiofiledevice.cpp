/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <gio/gio.h>

#include "dgiofiledevice.h"
#include "private/dfiledevice_p.h"
#include "dabstractfilewatcher.h"

DFM_BEGIN_NAMESPACE

class DGIOFileDevicePrivate : public DFileDevicePrivate
{
public:
    DGIOFileDevicePrivate(DGIOFileDevice *qq);

    Q_DECLARE_PUBLIC(DGIOFileDevice)

    GFile *file = nullptr;
    GInputStream *input_stream = nullptr;
    GOutputStream *output_stream = nullptr;
    GIOStream *total_stream = nullptr;
};

DGIOFileDevicePrivate::DGIOFileDevicePrivate(DGIOFileDevice *qq)
    : DFileDevicePrivate(qq)
{

}

DGIOFileDevice::DGIOFileDevice(const DUrl &url, QObject *parent)
    : DFileDevice(*new DGIOFileDevicePrivate(this), parent)
{
    setFileUrl(url);
}

DGIOFileDevice::~DGIOFileDevice()
{
    close();

    Q_D(DGIOFileDevice);

    if (d->file) {
        g_object_unref(d->file);
    }
}

bool DGIOFileDevice::open(QIODevice::OpenMode mode)
{
    if (isOpen())
        return false;

    if ((mode & ~(ReadWrite | Append | Truncate)) != 0)
        return false;

    Q_D(DGIOFileDevice);

    GError *error = nullptr;

    if (mode.testFlag(ReadWrite) && !mode.testFlag(Append)) {
        if (mode.testFlag(Truncate)) {
            d->total_stream = G_IO_STREAM(g_file_replace_readwrite(d->file, nullptr, false, G_FILE_CREATE_NONE, nullptr, &error));
        } else {
            d->total_stream = G_IO_STREAM(g_file_open_readwrite(d->file, nullptr, &error));
        }

        if (error) {
            setErrorString(QString::fromLocal8Bit(error->message));

            g_error_free(error);

            return false;
        }

        d->input_stream = g_io_stream_get_input_stream(d->total_stream);
        d->output_stream = g_io_stream_get_output_stream(d->total_stream);

        return true;
    }

    if (mode.testFlag(ReadOnly)) {
        d->input_stream = G_INPUT_STREAM(g_file_read(d->file, nullptr, &error));

        if (error) {
            setErrorString(QString::fromLocal8Bit(error->message));

            g_error_free(error);

            return false;
        }
    } else {
        d->input_stream = nullptr;
    }

    if ((mode & (WriteOnly | Append | Truncate))) {
        bool exists = g_file_query_exists(d->file, nullptr);
        if (mode.testFlag(Append)) {
            d->output_stream = G_OUTPUT_STREAM(g_file_append_to(d->file, G_FILE_CREATE_NONE, nullptr, &error));
        } else {
            d->output_stream = G_OUTPUT_STREAM(g_file_replace(d->file, nullptr, false, G_FILE_CREATE_NONE, nullptr, &error));
        }

        if (error) {
            do {
                if (error->domain == G_IO_ERROR && error->code == G_IO_ERROR_NOT_FOUND) {
                    g_error_free(error);
                    error = nullptr;
                    // 文件不存在, 尝试创建
                    d->output_stream = G_OUTPUT_STREAM(g_file_create(d->file, G_FILE_CREATE_NONE, nullptr, &error));

                    if (!error) {
                        break;
                    }
                }

                setErrorString(QString::fromLocal8Bit(error->message));

                g_error_free(error);

                if (d->input_stream) {
                    g_input_stream_close(d->input_stream, nullptr, nullptr);
                    g_object_unref(d->input_stream);
                }

                return false;
            } while (false);
        }

        if (!exists) {
            DAbstractFileWatcher::ghostSignal(d->url.parentUrl(), &DAbstractFileWatcher::subfileCreated, d->url);
        } else if (!mode.testFlag(Append) || mode.testFlag(Truncate)) {
            if (g_seekable_tell(G_SEEKABLE(d->output_stream)) > 0) {
                if (!g_seekable_can_seek(G_SEEKABLE(d->output_stream))) {
                    setErrorString("Can not seek the file, only append write mode");
                } else if (!g_seekable_seek(G_SEEKABLE(d->output_stream), 0, G_SEEK_SET, nullptr, &error)) {
                    if (error) {
                        setErrorString(QString::fromLocal8Bit(error->message));

                        g_error_free(error);
                    } else {
                        setErrorString("Failed on seek to start");
                    }
                } else {
                    return DFileDevice::open(mode);
                }

                if (d->input_stream) {
                    g_input_stream_close(d->input_stream, nullptr, nullptr);
                    g_object_unref(d->input_stream);
                }

                g_output_stream_close(d->output_stream, nullptr, nullptr);
                g_object_unref(d->output_stream);

                return false;
            }
        }
    } else {
        d->output_stream = nullptr;
    }

    return DFileDevice::open(mode);
}

void DGIOFileDevice::close()
{
    if (!isOpen())
        return;

    DFileDevice::close();

    Q_D(DGIOFileDevice);

    if (d->total_stream) {
        g_io_stream_close(d->total_stream, nullptr, nullptr);
        g_object_unref(d->total_stream);

        d->total_stream = nullptr;
        d->input_stream = nullptr;
        d->output_stream = nullptr;
    } else {
        if (d->input_stream) {
            g_input_stream_close(d->input_stream, nullptr, nullptr);
            g_object_unref(d->input_stream);

            d->input_stream = nullptr;
        }

        if (d->output_stream) {
            g_output_stream_close(d->output_stream, nullptr, nullptr);
            g_object_unref(d->output_stream);

            d->output_stream = nullptr;
        }
    }
}

bool DGIOFileDevice::setFileUrl(const DUrl &url)
{
    Q_ASSERT(!isOpen());
    DFileDevice::setFileUrl(url);

    Q_D(DGIOFileDevice);

    if (d->file) {
        g_object_unref(d->file);
    }

    if (url.isLocalFile()) {
        d->file = g_file_new_for_path(QFile::encodeName(url.toLocalFile()));
    } else {
        d->file = g_file_new_for_uri(QFile::encodeName(url.toString()));
    }

    return d->file;
}

int DGIOFileDevice::handle() const
{
    return -1;
}

qint64 DGIOFileDevice::size() const
{
    Q_D(const DGIOFileDevice);

    GFileInfo *info = g_file_query_info(d->file, nullptr, G_FILE_QUERY_INFO_NONE, nullptr, nullptr);

    if (!info) {
        return -1;
    }

    qint64 size = g_file_info_get_size(info);

    g_object_unref(info);

    return size;
}

bool DGIOFileDevice::resize(qint64 size)
{
    Q_D(DGIOFileDevice);

    GError *error = nullptr;

    if (!g_seekable_truncate(G_SEEKABLE(d->output_stream), size, nullptr, &error)) {
        if (error) {
            setErrorString(QString::fromLocal8Bit(error->message));
            g_error_free(error);
        } else {
            setErrorString("Failed on resize");
        }

        return false;
    }

    return true;
}

bool DGIOFileDevice::isSequential() const
{
    Q_D(const DGIOFileDevice);

    if (d->input_stream)
        return !g_seekable_can_seek(G_SEEKABLE(d->input_stream));

    return !g_seekable_can_seek(G_SEEKABLE(d->output_stream));
}

qint64 DGIOFileDevice::pos() const
{
    Q_D(const DGIOFileDevice);

    if (d->input_stream)
        return g_seekable_tell(G_SEEKABLE(d->input_stream));

    return g_seekable_tell(G_SEEKABLE(d->output_stream));
}

bool DGIOFileDevice::seek(qint64 pos)
{
    Q_D(DGIOFileDevice);

    GError *error = nullptr;

    if (d->input_stream) {
        if (!g_seekable_seek(G_SEEKABLE(d->input_stream), pos, G_SEEK_SET, nullptr, &error)) {
            if (error) {
                setErrorString(QString::fromLocal8Bit(error->message));
                g_error_free(error);
            } else {
                setErrorString("Failed on seek input stream");
            }

            return false;
        }
    }

    if (d->output_stream) {
        if (!g_seekable_seek(G_SEEKABLE(d->output_stream), pos, G_SEEK_SET, nullptr, &error)) {
            if (error) {
                setErrorString(QString::fromLocal8Bit(error->message));
                g_error_free(error);
            } else {
                setErrorString("Failed on seek output stream");
            }

            return false;
        }
    }

    return true;
}

bool DGIOFileDevice::flush()
{
    Q_D(DGIOFileDevice);

    GError *error = nullptr;
    bool ok = g_output_stream_flush(d->output_stream, nullptr, &error);

    if (error) {
        setErrorString(QString::fromLocal8Bit(error->message));

        g_error_free(error);

        return false;
    }

    return ok;
}

bool DGIOFileDevice::syncToDisk()
{
    return flush();
}

qint64 DGIOFileDevice::readData(char *data, qint64 maxlen)
{
    Q_D(DGIOFileDevice);

    GError *error = nullptr;
    qint64 size = g_input_stream_read(d->input_stream, data, maxlen, nullptr, &error);

    if (error) {
        setErrorString(QString::fromLocal8Bit(error->message));

        g_error_free(error);

        return -1;
    }

    return size;
}

qint64 DGIOFileDevice::writeData(const char *data, qint64 len)
{
    Q_D(DGIOFileDevice);

    GError *error = nullptr;
    qint64 size = g_output_stream_write(d->output_stream, data, len, nullptr, &error);

    if (error) {
        setErrorString(QString::fromLocal8Bit(error->message));

        g_error_free(error);

        return -1;
    }

    return size;
}

DFM_END_NAMESPACE
