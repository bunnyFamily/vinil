/** 
 *  @file     vhd.c
 *  @author   Igor Bonadio
 */

#include "vhd.h"

#include <stdlib.h>

#define htonll(value) ((u64)(htonl((u32)(value & 0xFFFFFFFFLL))) << 32) | htonl((u32)(value >> 32))

int vinil_checksum_vhd_footer(VHDFooter* vhd_footer) {
  unsigned char* buffer;
  buffer = (unsigned char*)vhd_footer;
  
  u32 temp_checksum = vhd_footer->checksum;
  vhd_footer->checksum = 0;
  
  u32 checksum = 0;
  int i;
  for (i = 0; i < sizeof(VHDFooter); i++)
    checksum += (u32)buffer[i];
  
  vhd_footer->checksum = temp_checksum;
  
  return ~checksum;
}

void vinil_vhd_footer_to_little_endian(VHDFooter* vhd_footer) {
  vhd_footer->features = htonl(vhd_footer->features);
	vhd_footer->file_format_version = htonl(vhd_footer->file_format_version);
	vhd_footer->data_offset = htonll(vhd_footer->data_offset);
	vhd_footer->timestamp = htonl(vhd_footer->timestamp);
	vhd_footer->creator_version = htonl(vhd_footer->creator_version);
	vhd_footer->creator_host_os = htonl(vhd_footer->creator_host_os);
	vhd_footer->original_size = htonll(vhd_footer->original_size);
	vhd_footer->current_size = htonll(vhd_footer->current_size);
	vhd_footer->disk_geometry = htonl(vhd_footer->disk_geometry);
	vhd_footer->disk_type = htonl(vhd_footer->disk_type);
  vhd_footer->checksum = htonl(vhd_footer->checksum);
}

VHD* vinil_vhd_open(const char* filename) {
  int error;
  
  VHD* vhd = (VHD*)malloc(sizeof(VHD));
  if (vhd == NULL)
    return NULL;
  
  vhd->fd = fopen(filename, "rb+");
  if (vhd->fd == NULL) {
    vinil_vhd_close(vhd);
    return NULL;
  }
  
  vhd->footer = (VHDFooter*)malloc(sizeof(VHDFooter));
  if (vhd->footer == NULL) {
    vinil_vhd_close(vhd);
    return NULL;
  }
  
  error = fseek(vhd->fd, 0, SEEK_END);
  if (error) {
    vinil_vhd_close(vhd);
    return NULL;
  }
  
  error = fseek(vhd->fd, ftell(vhd->fd) - sizeof(VHDFooter), SEEK_SET);
  if (error) {
    vinil_vhd_close(vhd);
    return NULL;
  }
  
  int b = fread(vhd->footer, sizeof(char), sizeof(VHDFooter), vhd->fd);
  if (b != sizeof(VHDFooter)) {
    vinil_vhd_close(vhd);
    return NULL;
  }
  
  vinil_vhd_footer_to_little_endian(vhd->footer);
  
  if (vinil_checksum_vhd_footer(vhd->footer) != vhd->footer->checksum) {
    vinil_vhd_close(vhd);
    return NULL;
  }
  
  return vhd;
}

void vinil_vhd_close(VHD* vhd) {
  fclose(vhd->fd);
  free(vhd->footer);
  free(vhd);
}