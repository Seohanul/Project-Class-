from pico2d import *

import game_framework
import main_state


name = "PauseState"
image = None
logo_time = 0.0
counter = 0

def enter():
    global image
    image = load_image('Pause.png')


def exit():
    global image
    del(image)
    pass

def update(frame_time):
    global counter
    counter = (counter + 1) % 100
    pass


def draw(frame_time):
    global image
    clear_canvas()
    main_state.draw_world()
    if counter < 50:
        image.draw(400, 300)
    update_canvas()
    pass


def handle_events(frame_time):
    events = get_events()
    for event in events:
        if event.type == SDL_KEYDOWN and event.key == SDLK_p:
            game_framework.pop_state()
    pass


def pause():
    pass


def resume():
    pass




