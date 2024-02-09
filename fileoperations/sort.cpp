/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *               2016 ~ 2018 dragondjf
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             zccrs<zhangjide@deepin.com>
 *             Tangtong<tangtong@deepin.com>
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
#include "sort.h"

typedef struct
{
  char *name;
  ino_t ino;
} direntry_t;

void *
x2nrealloc (void *p, size_t *pn, size_t s)
{
  size_t n = *pn;

  if (! p)
    {
      if (! n)
        {
          /* The approximate size to use for initial small allocation
             requests, when the invoking code specifies an old size of
             zero.  This is the largest "small" request for the GNU C
             library malloc.  */
          enum { DEFAULT_MXFAST = 64 * sizeof (size_t) / 4 };

          n = DEFAULT_MXFAST / s;
          n += !n;
        }
    }
  else
    {
      /* Set N = floor (1.5 * N) + 1 so that progress is made even if N == 0.
         Check for overflow, so that N * S stays in size_t range.
         The check may be slightly conservative, but an exact check isn't
         worth the trouble.  */
      if ((size_t) -1 / 3 * 2 / s <= n)
        exit(-1);
      n += n / 2 + 1;
    }

  *pn = n;
  return realloc (p, n * s);
}


/* Compare the names of two directory entries */

static int
direntry_cmp_name (void const *a, void const *b)
{
  direntry_t const *dea = (direntry_t const *)a;
  direntry_t const *deb = (direntry_t const *)b;

  return strcmp (dea->name, deb->name);
}

static int
direntry_cmp_inode (void const *a, void const *b)
{
  direntry_t const *dea = (direntry_t const *)a;
  direntry_t const *deb = (direntry_t const *)b;

  return dea->ino < deb->ino ? -1 : dea->ino > deb->ino;
}

typedef int (*comparison_function) (void const *, void const *);

static comparison_function const comparison_function_table[] =
  {
    0,
    direntry_cmp_name
   , direntry_cmp_inode
  };

/* Return a freshly allocated string containing the file names
   in directory DIRP, separated by '\0' characters;
   the end is marked by two '\0' characters in a row.
   Returned values are sorted according to OPTION.
   Return NULL (setting errno) if DIRP cannot be read.
   If DIRP is NULL, return NULL without affecting errno.  */

char *
streamsavedir (DIR *dirp)
{
  char *name_space = NULL;
  size_t allocated = 0;
  direntry_t *entries = NULL;
  size_t entries_allocated = 0;
  size_t entries_used = 0;
  size_t used = 0;
  int readdir_errno;
  comparison_function cmp = direntry_cmp_inode;

  if (dirp == NULL)
    return NULL;

  for (;;)
    {
      struct dirent const *dp;
      char const *entry;

      errno = 0;
      dp = readdir (dirp);
      if (! dp)
        break;

      /* Skip "", ".", and "..".  "" is returned by at least one buggy
         implementation: Solaris 2.4 readdir on NFS file systems.  */
      entry = dp->d_name;
      if (entry[entry[0] != '.' ? 0 : entry[1] != '.' ? 1 : 2] != '\0')
        {
          size_t entry_size = _D_EXACT_NAMLEN (dp) + 1;
          if (cmp)
            {
              if (entries_allocated == entries_used)
                {
                  size_t n = entries_allocated;
                  entries = (direntry_t *)x2nrealloc (entries, &n, sizeof *entries);
                  entries_allocated = n;
                }
              entries[entries_used].name = strdup (entry);

              entries[entries_used].ino = dp->d_ino;

              entries_used++;
            }
          else
            {
              if (allocated - used <= entry_size)
                {
                  size_t n = used + entry_size;
                  if (n < used)
                    exit(-1);
                  name_space = (char *)x2nrealloc (name_space, &n, 1);
                  allocated = n;
                }
              memcpy (name_space + used, entry, entry_size);
            }
          used += entry_size;
        }
    }

  readdir_errno = errno;
  if (readdir_errno != 0)
    {
      free (entries);
      free (name_space);
      errno = readdir_errno;
      return NULL;
    }

  if (cmp)
    {
      size_t i;

      if (entries_used)
        qsort (entries, entries_used, sizeof *entries, cmp);
      name_space = (char *)malloc (used + 1);
      used = 0;
      for (i = 0; i < entries_used; i++)
        {
          char *dest = name_space + used;
          used += stpcpy (dest, entries[i].name) - dest + 1;
          free (entries[i].name);
        }
      free (entries);
    }
  else if (used == allocated)
    name_space = (char *)realloc (name_space, used + 1);

  name_space[used] = '\0';
  return name_space;
}

/* Return a freshly allocated string containing the file names
   in directory DIR, separated by '\0' characters;
   the end is marked by two '\0' characters in a row.
   Return NULL (setting errno) if DIR cannot be opened, read, or closed.  */

char *savedir (char const *dir)
{
  DIR *dirp = opendir (dir);
  if (! dirp)
    return NULL;
  else
    {
      char *name_space = streamsavedir (dirp);
      if (closedir (dirp) != 0)
        {
          int closedir_errno = errno;
          free (name_space);
          errno = closedir_errno;
          return NULL;
        }
      return name_space;
    }
}
