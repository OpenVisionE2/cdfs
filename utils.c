/*
  
  File utils.c - miscellaneous routines for cdfs

  
  Copyright (c) 1999, 2000, 2001 by Michiel Ronsse 
  
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
  
*/


#include "cdfs.h"


/* convert YYYYMMDDHHMMSS to time_t (seconds since 1/1/1970) */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0))
time64_t cdfs_constructtime(char * time)
#else
time_t cdfs_constructtime(char * time)
#endif
{
  
  unsigned year,month,day,hour,min,sec;

  if (!time) return 0;
  if ((*time!='1') && (*time!='2')) return 0;

  year  = 1000*(time[0]-'0')+100*(time[1]-'0')+10*(time[2]-'0')+(time[3]-'0');

  if (year<1970)
    return 0;
  else {
    year -= 1970;
    month = 10*(time[ 4]-'0') + (time[ 5]-'0');
    day   = 10*(time[ 6]-'0') + (time[ 7]-'0');
    hour  = 10*(time[ 8]-'0') + (time[ 9]-'0');
    min   = 10*(time[10]-'0') + (time[11]-'0');
    sec   = 10*(time[12]-'0') + (time[13]-'0');

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0))
    return mktime64(year, month, day, hour, min, sec);
#else
    return mktime(year, month, day, hour, min, sec);
#endif
  }
}

/****************************************/

unsigned cdfs_constructsize(char * size){
  return (unsigned char)size[0]+
    ((unsigned char)size[1]<<8)+
    ((unsigned char)size[2]<<16)+
    ((unsigned char)size[3]<<24);
}

/**************************/


void cdfs_constructMSFsize(char * result, unsigned length){
  int min,sec;
  length = (length-WAV_HEADER_SIZE)/CD_FRAMESIZE_RAW;
  length /= 75;
  sec = length%60;
  min = length/60;
  sprintf(result,"%2d:%02d", min, sec);
}

/***********************************************************************/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0))
int ioctl_by_bdev(struct block_device *bdev, unsigned cmd, unsigned long arg)
{
       int res;
       mm_segment_t old_fs = get_fs();
       set_fs(KERNEL_DS);
       res = blkdev_ioctl(bdev, 0, cmd, arg);
       set_fs(old_fs);
       return res;
}
#endif

int cdfs_ioctl(struct super_block *s, int cmd, unsigned long arg) {
  return ioctl_by_bdev(s->s_bdev, cmd, arg); 
}

/************************/
