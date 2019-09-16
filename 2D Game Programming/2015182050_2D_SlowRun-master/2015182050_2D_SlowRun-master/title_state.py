from pico2d import *
import game_framework
import main_state

class MusicPlayer:
    def __init__(self):
        self.bgm = load_music('./Resources/music/bgm_main.wav')
        self.bgm.set_volume(64)
        self.bgm.repeat_play()
        self.start_sound = load_wav('./Resources/music/start.wav')
        self.start_sound.set_volume(32)


name = "TitleState"
image = None
music = None

def create_world():
    global music
    music = MusicPlayer()
    global image
    image = load_image('./Resources/image/map/start.png')


def enter():
    create_world()


def exit():
    destroy_world()


def destroy_world():
    global image
    del(image)


def handle_events(frame_time):
    events = get_events()
    for event in events:
        if event.type == SDL_QUIT:
            game_framework.quit()
        else:
            if(event.type, event.key) == (SDL_KEYDOWN, SDLK_ESCAPE):
                game_framework.quit()
            elif (event.type, event.key) == (SDL_KEYDOWN, SDLK_SPACE):
                music.start_sound.play()

                game_framework.change_state(main_state)


def draw(frame_time):
    image.draw(400,300)
    update_canvas()


def update(frame_time):
    pass


def pause():
    pass


def resume():
    pass






