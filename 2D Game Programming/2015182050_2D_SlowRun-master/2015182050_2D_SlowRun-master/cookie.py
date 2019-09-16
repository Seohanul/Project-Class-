import random

from pico2d import *
from obstacle import level
import game_framework
import title_state

class Cookie:
    PIXEL_PER_METER = (30.0 / 0.3)           # 100p / m

    JUMP_SPEED_KMPH = 14.4  # Km / Hour
    JUMP_SPEED_MPM = (JUMP_SPEED_KMPH * 1000.0 / 60.0)  # 240m/m
    JUMP_SPEED_MPS = (JUMP_SPEED_MPM / 60.0)    # 4m/s
    JUMP_SPEED_PPS = (JUMP_SPEED_MPS * PIXEL_PER_METER)

    LEVEL_SPEED_KMPH = 10.8  # Km / Hour
    LEVEL_SPEED_MPM = (LEVEL_SPEED_KMPH * 1000.0 / 60.0)  # 180m/m
    LEVEL_SPEED_MPS = (LEVEL_SPEED_MPM / 60.0)  # 3m/s
    LEVEL_SPEED_PPS = (LEVEL_SPEED_MPS * PIXEL_PER_METER)

    TIME_PER_ACTION = 0.5
    ACTION_PER_TIME = 1.0 / TIME_PER_ACTION
    FRAMES_PER_ACTION = 8
    MAX_JUMP = 1.3  # 1m
    MAX_JUMP2 = 0
    BOTTOM = 2.45 # 2.45m
    RUN, JUMPING_UP, JUMPING_DOWN, SLIDE, JUMPING_UP2, JUMPING_DOWN2 = 0, 1, 2, 3, 4, 5
    UNBEATABLE, BEATABLE, GIANT, QUICK = 0, 1, 2, 3
    def __init__(self):
        self.x, self.y = 60, self.BOTTOM * self.PIXEL_PER_METER
        self.frame = random.randint(0, 5)
        self.total_frames = 0.0
        self.up = 0
        self.stage = 1
        self.state = self.RUN
        self.lifestate = self.BEATABLE
        self.life = 5
        self.gianttime = 0
        self.quicktime = 0
        self.grace_period = 2
        self.life_time = 0
        self.giantrate = 1.0
        self.jump_height = self.MAX_JUMP * self.PIXEL_PER_METER + self.BOTTOM * self.PIXEL_PER_METER
        self.run_image = load_image('./Resources/image/charater/cookie_run.png')
        self.jump_image = load_image('./Resources/image/charater/cookie_run_jump.png')
        self.slide_image = load_image('./Resources/image/charater/cookie_run_slide.png')
        self.doublejump_image = load_image('./Resources/image/charater/cookie_double_jump.png')
        self.pluslife_image = load_image('./Resources/image/map/+life.png')
        self.minuslife_image = load_image('./Resources/image/map/-life.png')
        self.jump_sound = load_wav('./Resources/music/ch_jump.wav')
        self.jump_sound.set_volume(32)
        self.slide_sound = load_wav('./Resources/music/ch_slide.wav')
        self.slide_sound.set_volume(32)

    def update(self, frame_time):
        def clamp(minimum, x, maximum):
            return max(minimum, min(x, maximum))

        Cookie.JUMP_SPEED_KMPH = 14.4 + level *level * Cookie.LEVEL_SPEED_KMPH
        Cookie.JUMP_SPEED_MPM = (Cookie.JUMP_SPEED_KMPH * 1000.0 / 60.0)
        Cookie.JUMP_SPEED_MPS = (Cookie.JUMP_SPEED_MPM / 60.0)
        Cookie.JUMP_SPEED_PPS = (Cookie.JUMP_SPEED_MPS * Cookie.PIXEL_PER_METER)
        distance = Cookie.JUMP_SPEED_PPS * frame_time
        self.total_frames += Cookie.FRAMES_PER_ACTION * Cookie.ACTION_PER_TIME * frame_time
        self.frame = int(self.total_frames) % 6
        self.x = clamp(0, self.x, 800)
        self.y += (self.up * distance)


        if self.state == self.JUMPING_UP:
            self.jump_sound.play()
            self.up = 1
            if self.y > self.jump_height:
                self.state = self.JUMPING_DOWN
                self.up = -1

        elif self.state == self.JUMPING_DOWN or self.state == self.JUMPING_DOWN2:
            if self.y < self.BOTTOM * self.PIXEL_PER_METER:
                self.state = self.RUN
                self.up = 0

        elif self.state == self.SLIDE:

            self.up = 0
            self.y = self.BOTTOM * self.PIXEL_PER_METER

        elif self.state == self.JUMPING_UP2:
            self.jump_sound.play()
            self.up = 1
            if self.y > self.MAX_JUMP2:
                self.state = self.JUMPING_DOWN2
                self.up = -1

        if self.lifestate == self.UNBEATABLE:
            self.life_time += frame_time
            if self.life_time >= self.grace_period:
                self.life_time = 0
                self.lifestate = self.BEATABLE

        if self.lifestate == self.GIANT:
            self.gianttime += frame_time
            if self.gianttime >= 4:
                self.gianttime = 0
                self.lifestate = self.UNBEATABLE

        if self.lifestate == self.QUICK:
            self.quicktime += frame_time
            Cookie.ACTION_PER_TIME = 10

            if self.quicktime >= 4:
                self.quicktime = 0
                self.lifestate = self.UNBEATABLE
                Cookie.ACTION_PER_TIME = 2

    def sub_life(self):
        self.life -= 1
        self.lifestate = self.UNBEATABLE
        if self.life == 0:
            exit()

    def plus_life(self):
        self.life += 1
        if self.life == 6:
            self.life -= 1
        pass

    def get_giant(self):
        self.lifestate = self.GIANT
        pass

    def get_fast(self):
        self.lifestate = self.QUICK
        pass


    def draw(self):
        if self.lifestate == self.BEATABLE or self.lifestate == self.QUICK:
            if self.state == self.RUN:
                self.run_image.opacify(1)
                self.run_image.clip_draw(self.frame * 75, 0, 75, 87, self.x, self.y)
            elif (self.state == self.JUMPING_UP) or (self.state == self.JUMPING_DOWN):
                self.jump_image.opacify(1)
                self.jump_image.clip_draw(0, 0, 75, 87, self.x, self.y)
            elif self.state == self.SLIDE:
                self.slide_image.opacify(1)
                self.slide_image.clip_draw(0, 0, 90, 45, self.x, self.y - 20)
            elif (self.state == self.JUMPING_DOWN2) or (self.state == self.JUMPING_UP2):
                self.doublejump_image.opacify(1)
                self.doublejump_image.clip_draw(self.frame * 87, 0, 87, 87, self.x, self.y)
        elif self.lifestate == self.UNBEATABLE or self.lifestate == self.QUICK:
            if self.state == self.RUN:
                self.run_image.opacify(random.random())
                self.run_image.clip_draw(self.frame * 75, 0, 75, 87, self.x, self.y)
            elif (self.state == self.JUMPING_UP) or (self.state == self.JUMPING_DOWN):
                self.jump_image.opacify(random.random())
                self.jump_image.clip_draw(0, 0, 75, 87, self.x, self.y)
            elif self.state == self.SLIDE:
                self.slide_image.opacify(random.random())
                self.slide_image.clip_draw(0, 0, 90, 45, self.x, self.y - 20)
            elif (self.state == self.JUMPING_DOWN2) or (self.state == self.JUMPING_UP2):
                self.doublejump_image.opacify(random.random())
                self.doublejump_image.clip_draw(self.frame * 87, 0, 87, 87, self.x, self.y)
        elif self.lifestate == self.GIANT or self.lifestate == self.QUICK:
            if self.state == self.RUN:
                self.run_image.opacify(1)
                self.run_image.clip_draw(self.frame * 75, 0, 75, 87, self.x, self.y + 50, 75 * 2, 87 * 2)
            elif (self.state == self.JUMPING_UP) or (self.state == self.JUMPING_DOWN):
                self.jump_image.opacify(1)
                self.jump_image.clip_draw(0, 0, 75, 87, self.x, self.y + 87, 75 * 2, 87 * 2)
            elif self.state == self.SLIDE:
                self.slide_image.opacify(1)
                self.slide_image.clip_draw(0, 0, 90, 45, self.x, self.y - 20 + 25, 90 * 2, 45 * 2)
            elif (self.state == self.JUMPING_DOWN2) or (self.state == self.JUMPING_UP2):
                self.doublejump_image.opacify(1)
                self.doublejump_image.clip_draw(self.frame * 87, 0, 87, 87, self.x, self.y + 87, 87 * 2, 87 * 2)
        i = 5
        while i > 0:
            self.pluslife_image.draw(750 - i * 50, 750)
            i -= 1
        j = 5 - self.life
        while j > 0:
            self.minuslife_image.draw(750 - j * 50, 750)
            j -= 1


    def get_bb(self):
        if self.state == self.RUN:
            return self.x - 20, self.y - 40, self.x + 20, self.y + 40
        if (self.state == self.JUMPING_UP) or (self.state == self.JUMPING_DOWN) or (self.state == self.JUMPING_UP2) or (self.state == self.JUMPING_DOWN2):
            return self.x - 20, self.y - 30, self.x + 20, self.y + 30
        if self.state == self.SLIDE:
            return self.x - 45, self.y - 42.5, self.x + 45, self.y + 2.5
        pass

    def draw_bb(self):
        draw_rectangle(*self.get_bb())

    def handle_event(self, event):
        if (event.type, event.key) == (SDL_KEYDOWN, SDLK_UP) and (self.state == self.RUN or self.state == self.SLIDE):
            self.state = self.JUMPING_UP
            Cookie.ACTION_PER_TIME = 5

        elif (event.type, event.key) == (SDL_KEYDOWN, SDLK_DOWN) and self.state != self.JUMPING_UP and self.state != self.JUMPING_UP2:
            self.slide_sound.play()
            self.state = self.SLIDE
            Cookie.ACTION_PER_TIME = 5

        elif (event.type, event.key) == (SDL_KEYUP, SDLK_DOWN) and self.state == self.SLIDE:
            self.state = self.RUN
            Cookie.ACTION_PER_TIME = 2
        elif (event.type, event.key) == (SDL_KEYDOWN, SDLK_UP) and (self.state == self.JUMPING_UP or self.state == self.JUMPING_DOWN):
            self.state = self.JUMPING_UP2
            self.MAX_JUMP2 = self.y + self.MAX_JUMP * self.PIXEL_PER_METER
            Cookie.ACTION_PER_TIME = 2