#include <stdio.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

struct partition {
	u8 drive;             /* drive number FD=0, HD=0x80, etc. */

	u8  head;             /* starting head */
	u8  sector;           /* starting sector */
	u8  cylinder;         /* starting cylinder */

	u8  sys_type;         /* partition type: NTFS, LINUX, etc. */

	u8  end_head;         /* end head */
	u8  end_sector;       /* end sector */
	u8  end_cylinder;     /* end cylinder */

	u32 start_sector;     /* starting sector counting from 0 */
	u32 nr_sectors;       /* number of of sectors in partition */
};

int main(int argc, char *argv[], char *env[])
{
	char buf[512];
	int fd = open("vdisk", 0);
	int extended, d, cond;
	read(fd, buf, 512);
	struct partition *p = (struct partition *) &buf[0x1BE];
	struct partition *EXT;
	printf("%s %14s %14s %14s %15s %14s\n", "Device", "Boot Start", "End", "Sectors", "Size", "Id Type");
	for(int i = 0; i < 4; i++){
		if (p->sys_type == 5){
			extended = p->start_sector;
			cond = ((p->start_sector + p->nr_sectors) - 1); 
		}
		printf("vdisk%d", i + 1);
		printf("%15d", p->start_sector);
		printf("%15d", (p->start_sector + p->nr_sectors) - 1);
		printf("%15d", p->nr_sectors);
		printf("%15dk", (p->nr_sectors)/2);
		printf("%15x\n", p->sys_type);
		p++;
		d = i;
	}
	d += 2;
	while(extended < cond){	
		lseek(fd, extended*512, SEEK_SET);
		read(fd, buf, 512);
		EXT = &buf[0x1BE];
		printf("vdisk%d", d++);
		printf("%15d", EXT->start_sector + extended);
		printf("%15d", (EXT->start_sector + EXT->nr_sectors) - 1 + extended);
		printf("%15d", EXT->nr_sectors);
		printf("%15dk", (EXT->nr_sectors)/2);
		printf("%15x\n", EXT->sys_type);
		extended += EXT->nr_sectors + EXT->start_sector;
	}
	return 0;

}
