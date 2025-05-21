
#include "cub3d.h"

int main(void)
{
	void *mlx;
	void *mlx_win;
  /* LUL parts to do */
  //TODO: check for argument
  //TODO: Map parsing
  //TODO: .cub file format parsing
  //TODO: Setting up error handling
  //TODO: Create initialize data
  //TODO: Connect to the mlx function
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world");
	mlx_loop(mlx);
}
