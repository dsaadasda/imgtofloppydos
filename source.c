#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define SECTOR_SIZE 512
#define FLOPPY_DRIVE 0
#define MAX_FILENAME 256


void draw_progress(unsigned long current, unsigned long total) {
    int width = 50;
    int filled;
    int i;

    filled = (int)((current * width) / total);

    printf("\r[");
    for (i = 0; i < width; i++) {
        if (i < filled) putchar('=');
        else putchar(' ');
    }
    printf("] %lu/%lu sectors", current, total);
    fflush(stdout);
}

int write_sector(unsigned long sector, unsigned char *buffer) {
    union REGS regs;
    struct SREGS sregs;
    unsigned int cyl, head, sec;
    int retry;

    cyl = (unsigned int)(sector / 18);
    head = (unsigned int)((sector / 18) % 2);
    sec = (unsigned int)((sector % 18) + 1);

    regs.h.ah = 0x03;
    regs.h.al = 1;
    regs.h.ch = (unsigned char)cyl;
    regs.h.cl = (unsigned char)sec;
    regs.h.dh = (unsigned char)head;
    regs.h.dl = FLOPPY_DRIVE;
    sregs.es = FP_SEG(buffer);
    regs.x.bx = FP_OFF(buffer);

    for (retry = 0; retry < 3; retry++) {
        int86x(0x13, &regs, &regs, &sregs);
        if (!(regs.x.cflag & 1)) return 0;
    }

    return 1;
}

int main(void) {
    FILE *img;
    unsigned char buffer[SECTOR_SIZE];
    unsigned long filesize;
    unsigned long total_sectors;
    unsigned long sector, i;
    char filename[MAX_FILENAME];

    clrscr();
    printf("=== DOS Floppy Writer GUI ===\n\n");

    printf("Enter path to IMG file: ");
    scanf("%255s", filename);

    img = fopen(filename, "rb");
    if (!img) {
        printf("Cannot open IMG file!\n");
        return 1;
    }

    fseek(img, 0, SEEK_END);
    filesize = ftell(img);
    fseek(img, 0, SEEK_SET);

    total_sectors = (filesize + SECTOR_SIZE - 1) / SECTOR_SIZE;

    printf("IMG size: %lu bytes\n", filesize);
    printf("Total sectors to write: %lu\n", total_sectors);
    printf("Writing to floppy drive A:\n\n");

    for (sector = 0; sector < total_sectors; sector++) {
        size_t read;
        read = fread(buffer, 1, SECTOR_SIZE, img);
        if (read < SECTOR_SIZE) {
            for (i = read; i < SECTOR_SIZE; i++) buffer[i] = 0;
        }

        if (write_sector(sector, buffer)) {
            printf("\nFailed writing sector %lu!\n", sector);
            fclose(img);
            return 2;
        }

        draw_progress(sector + 1, total_sectors);
    }

    fclose(img);
    printf("\n\nWrite completed successfully!\n");
    return 0;
}
