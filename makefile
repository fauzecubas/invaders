ALLEGRO_FLAGS = -lallegro -lallegro_font -lallegro_primitives -lallegro_image

all:
	gcc -o invaders main.c libs.c $(ALLEGRO_FLAGS) -g

clean:
	rm -rf invaders