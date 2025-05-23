
// // file to check the keycodes

// #include "../inc/cub3d.h"

// #define UP_ARROW 65362
// #define DOWN_ARROW 65364
// #define LEFT_ARROW 65361
// #define RIGHT_ARROW 65363

// #define SPACEBAR 32
// #define E 101

// #define L_MOUSE 1
// #define R_MOUSE 3

// int key_press(int key)
// {
// 	printf("%d\n", key);
// 	return 0;
// }

// int mouse(int key)
// {
// 	printf("%d\n", key);
// 	return 0;
// }

// int main(void)
// {
// 	void *mlx = mlx_init();
// 	void *win = mlx_new_window(mlx, 1280, 720, "test");

// 	mlx_key_hook(win, key_press, NULL);
// 	mlx_mouse_hook(win, mouse, NULL);
// 	mlx_loop(mlx);
// }