from pico2d import *

import game_framework
import title_state

from cookie import Cookie # import Boy class from boy.py
from obstacle import Grass
from obstacle import Background
from obstacle import ObjGenerator

name = "collision"

cookie = None
grass = None
background = None
generator = None

def create_world():
    global cookie, grass, background, generator
    cookie = Cookie()
    grass = Grass()
    background = Background()
    generator = ObjGenerator()
    pass


def destroy_world():
    global cookie, grass, background, spears, thorns, generator

    del(cookie)
    del(grass)
    del(background)
    del(generator)

def enter():
    open_canvas(800, 800)
    game_framework.reset_time()
    create_world()


def exit():
    destroy_world()
    close_canvas()


def pause():
    pass


def resume():
    pass


def handle_events(frame_time):
    events = get_events()
    for event in events:
        if event.type == SDL_QUIT:
            game_framework.quit()
        else:
            if (event.type, event.key) == (SDL_KEYDOWN, SDLK_ESCAPE):
                game_framework.quit()
            else:
                cookie.handle_event(event)


def collide(a, b):
    left_a, bottom_a, right_a, top_a = a.get_bb()
    left_b, bottom_b, right_b, top_b = b.get_bb()

    if left_a > right_b:
        return False

    if right_a < left_b:
        return False

    if top_a < bottom_b:
        return False

    if bottom_a > top_b:
        return False

    return True
    pass


def update(frame_time):
    global cookie, generator
    cookie.update(frame_time)
    grass.update(frame_time)
    background.update(frame_time)
    generator.update(frame_time)
    object_list = generator.object_list
    for object in object_list:
        object.update(frame_time)
        if(collide(cookie, object)):
            if cookie.lifestate == cookie.BEATABLE:
                cookie.sub_life()
            elif cookie.lifestate == cookie.GIANT:
                object.eat()
            elif cookie.lifestate == cookie.QUICK:
                object.eat()

        if cookie.lifestate == cookie.QUICK:
            object.set_quickspeed()
            grass.set_quickspeed()
            background.set_quickspeed()
        elif cookie.lifestate != cookie.QUICK:
            if cookie.state == cookie.JUMPING_UP or cookie.state == cookie.JUMPING_DOWN or cookie.state == cookie.SLIDE:
                object.set_fastspeed()
                grass.set_fastspeed()

            elif cookie.state == cookie.RUN:
                object.set_slowspeed()
                grass.set_slowspeed()
                background.set_slowspeed()
    item_list = generator.item_list
    for object in item_list:
        object.update(frame_time)
        if(collide(cookie, object)):
            if object.type == 'Recover':
                cookie.plus_life()
                item_list.remove(object)

            elif object.type == 'Giant':
                cookie.get_giant()
                item_list.remove(object)

            elif object.type == 'FastRun':
                cookie.get_fast()
                item_list.remove(object)

def draw(frame_time):
    clear_canvas()
    background.draw()
    grass.draw()
    cookie.draw()
    object_list = generator.object_list
    item_list = generator.item_list
    for object in object_list:
        object.draw()
    for object in item_list:
        object.draw()
    generator.draw()
    grass.draw_bb()
    cookie.draw_bb()

    update_canvas()






