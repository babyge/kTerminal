/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kFile.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>

#include "kFile.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t kFile_GetPath( kfile_setting_t *setting )
{
    uint32_t lens = GetModuleFileName(NULL, setting->path, MAX_PATH);
    char *ptr = strrchr(setting->path, '\\');
    lens = ptr - setting->path + 1;
    setting->path[lens] = 0;
    // printf(">> path = %s, %d\n", setting->path, lens);
    return lens;
}

uint32_t kFile_GetSetting( kfile_setting_t *setting )
{
    uint32_t status;
    status = kFile_LoadSetting(setting);
    if (status != KS_OK)
    {
        if (cplist.num > 0)
        {
            s.port = cplist.port[0];
            status = kFile_UpdateSetting(setting);
        }
    }
    return status;
}

uint32_t kFile_LoadSetting( kfile_setting_t *cfg )
{
    FILE *fp;
    char fullname[MAX_PATH+1] = {0};

    sprintf(fullname, "%s%s", cfg->path, cfg->filename);
    fp = fopen(fullname, "rb");
    if (fp == NULL)
    {
        printf("  open setting file error ... create new\n");
        fp = fopen(fullname, "ab");
        *cfg->port     = DEFAULT_COMPORT;
        *cfg->baudrate = DEFAULT_BAUDRATE;
        *cfg->databits = DEFAULT_DATABITS;
        *cfg->stopbits = DEFAULT_STOPBITS;
        *cfg->parity   = DEFAULT_PARITY;
        *cfg->flowctrl = DEFAULT_FLOWCTRL;
        fprintf(fp, "%d,%d,%d,%c,%d,%d",
            *cfg->port, *cfg->baudrate,
            *cfg->databits, serial_parity_string_short[*cfg->parity], *cfg->stopbits, *cfg->flowctrl);
        fclose(fp);
        return KS_ERROR;
    }
    else
    {
        char mode[4] = {0};
        fscanf(fp, "%d,%d,%c,%c,%c,%c",
            cfg->port, cfg->baudrate,
            &mode[0], &mode[1], &mode[2], &mode[3]);
//         fprintf(fp, "%s",
//             cfg->logpath);
// printf("logpath = %s\n", cfg->logpath);
        *cfg->databits = mode[0] - '0';
        *cfg->stopbits = mode[2] - '0';
        *cfg->flowctrl = mode[3] - '0';
        switch (mode[1])
        {
            case 'N':   *cfg->parity = SERIAL_PARITY_NONE;
            case 'O':   *cfg->parity = SERIAL_PARITY_ODD;
            case 'E':   *cfg->parity = SERIAL_PARITY_EVEN;
            case 'M':   *cfg->parity = SERIAL_PARITY_MARK;
            case 'S':   *cfg->parity = SERIAL_PARITY_SPACE;
            default:    *cfg->parity = SERIAL_PARITY_NONE;
        }
        fclose(fp);
    }
    return KS_OK;
}

uint32_t kFile_UpdateSetting( kfile_setting_t *cfg )
{
    FILE *fp;
    char fullname[MAX_PATH+1] = {0};

    sprintf(fullname, "%s%s", cfg->path, cfg->filename);
    fp = fopen(fullname, "wb");
    if (fp == NULL)
    {
        return KS_ERROR;
    }
    else
    {
        fprintf(fp, "%d,%d,%d,%c,%d,%d",
            *cfg->port, *cfg->baudrate,
            *cfg->databits, serial_parity_string_short[*cfg->parity], *cfg->stopbits, *cfg->flowctrl);
        fclose(fp);
    }
    return KS_OK;
}

uint32_t kFile_CreateLogFile( void )
{
    return 0;
}

// void ist_db_get_timestamp( uint16_t *timestamp )
// {
//     time_t t = time(NULL);
//     struct tm tm = *localtime(&t);
//     timestamp[0] = tm.tm_year + 1900;
//     timestamp[1] = tm.tm_mon + 1;
//     timestamp[2] = tm.tm_mday;
//     timestamp[3] = tm.tm_hour;
//     timestamp[4] = tm.tm_min;
//     timestamp[5] = tm.tm_sec;
// }

// uint32_t ist_db_get_filename( char *filename, const char *info, uint16_t *ts, const char *extension )
// {
//     uint32_t lens = 0;

//     lens = sprintf(filename, "LOG_");
//     if (info != NULL)
//     {
//         lens += sprintf(filename + lens, "%s_", info);
//     }
//     lens += sprintf(filename + lens, "%04d%02d%02d_%02d%02d%02d", ts[0], ts[1], ts[2], ts[3], ts[4], ts[5]);
//     if (extension != NULL)
//     {
//         lens += sprintf(filename + lens, ".%s", extension);
//     }
//     return lens;
// }

// uint32_t ist_db_save_file( const char *path, const char *filename, const char *extension, const char *file )
// {
//     FILE *fp;
//     uint32_t lens = 0;
//     char fullfilename[128] = {0};
//     if (path != NULL)
//     {
//         lens += sprintf(fullfilename, "%s/", path);
//     }
//     sprintf(fullfilename + lens, "%s.%s", filename, extension);
//     printf(">> filename: %s\n", fullfilename);
//     fp = fopen(fullfilename, "w+");
//     if (fp == NULL)
//     {
//         return DB_ERROR;
//     }
//     fprintf(fp, "%s", file);
//     fclose(fp);
//     return DB_OK;
// }

/*************************************** END OF FILE ****************************************/
