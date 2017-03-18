#include <genesis.h>

#include "vdp_bg.h"
#include "vdp_pal.h"
#include "vdp_tile.h"
#include "vdp.h"
#include "bmp.h"
#include "tools.h"
#include "gfx.h"
#include "sprite_eng.h"
#include "vdp_dma.h"
#include "vdp_spr.h"
#include "dma.h"
#include "sprite_eng.h"
#include "vram.h"

int main()
{
	u16 palette[64];
	u16 ind;

	//disable interrupt when accessing VDP
	SYS_disableInts();
	// initialization
	VDP_setScreenWidth320();

	// init sprites engine
	SPR_init(16, 256, 256);

	// set all palette to black
	VDP_setPaletteColors(0, (u16*) palette_black, 64);

	// load image
	ind = TILE_USERINDEX;
	VDP_drawImageEx(PLAN_B, &bgb_image, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
	ind += bgb_image.tileset->numTile;
	VDP_drawImageEx(PLAN_A, &bga_image, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
	ind += bga_image.tileset->numTile;

	// prepare palettes
	memcpy(&palette[0], bgb_image.palette->data, 16 * 2);
	memcpy(&palette[16], bga_image.palette->data, 16 * 2);

	// fade in
	VDP_fadeIn(0, (4 * 16) - 1, palette, 20, FALSE);

	while(TRUE)
	{
		// update sprites
		SPR_update();

		VDP_waitVSync();
	}
	return 0;
}
