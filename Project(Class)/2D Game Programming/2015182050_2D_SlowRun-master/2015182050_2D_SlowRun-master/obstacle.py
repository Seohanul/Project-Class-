import random

from pico2d import *
#구현 계획
# 패턴을 여러개 만들자****
# 쉬운거, 중간거, 어려운거 여러개씩 만든다음에
# 레벨에 따라서 비율을 다르게 만드는 알고리즘을 구현하자
# 이미지를 다르게 하자
# spear2 thorn2 만들고 그걸 중급으로 만들고
# 고급은 거기서 여러개 섞어서 하는거로 만들자
# 레벨 10당 비율로 해서 10까진 초급 20까진 초급 중급 30까진 초급 중급 고급
# 30넘어가면 속도 유지 비율 변경


#아이템 구현해보자
# 거인되기
    # 거인 되는 과정 (크기 점점 커지고 점점 작아져야함
    # 작아질때는 또 무적이면서 깜빡거려야하고
    # 커져있는 상태는 무적이여야함

# 불꽃 달리기 등
    # 애니메이션이 달라야겠네?
    # 현재는 이미지 프레임 변화가 없으니까 속도가 빨라지는 느낌이 안듬
    # 프레임 변화 실험 몇가지 해보고 적용하자

#인터페이스 구현
    # 시작 인터페이스는 구현 완료

    #종료 인터페이스
        # 점수를 어떻게 표기할것인지 정하자


PIXEL_PER_METER = (30.0 / 0.3)  # 100p / m
level = 1

LEVEL_SPEED_KMPH = 1.8  # Km / Hour
LEVEL_SPEED_MPM = (LEVEL_SPEED_KMPH * 1000.0 / 60.0)  # 30m/m
LEVEL_SPEED_MPS = (LEVEL_SPEED_MPM / 60.0)  # 0.5m/s
LEVEL_SPEED_PPS = (LEVEL_SPEED_MPS * PIXEL_PER_METER)

BACKGROUND_SPEED_KMPH = 1.8  # Km / Hour
BACKGROUND_SPEED_MPM = (BACKGROUND_SPEED_KMPH * 1000.0 / 60.0)  # 30m/m
BACKGROUND_SPEED_MPS = (BACKGROUND_SPEED_MPM / 60.0)  # 0.5m/s
BACKGROUND_SPEED_PPS = (BACKGROUND_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level

SLOW_SPEED_KMPH = 1.8  # Km / Hour
SLOW_SPEED_MPM = (SLOW_SPEED_KMPH * 1000.0 / 60.0)  # 30m/m
SLOW_SPEED_MPS = (SLOW_SPEED_MPM / 60.0)  # 0.5m/s
SLOW_SPEED_PPS = (SLOW_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level

OBSTACLE_SPEED_KMPH = 10.8  # Km / Hour
OBSTACLE_SPEED_MPM = (OBSTACLE_SPEED_KMPH * 1000.0 / 60.0)  # 180m/m
OBSTACLE_SPEED_MPS = (OBSTACLE_SPEED_MPM / 60.0)  # 3m/s
OBSTACLE_SPEED_PPS = (OBSTACLE_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level


QUICK_SPEED_KMPH = 21.6  # Km / Hour
QUICK_SPEED_MPM = (QUICK_SPEED_KMPH * 1000.0 / 60.0)  # 360m/m
QUICK_SPEED_MPS = (QUICK_SPEED_MPM / 60.0)  # 6m/s
QUICK_SPEED_PPS = (QUICK_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level


REST, GENERATE = 0, 1
font = None
speed = SLOW_SPEED_MPS

class ObjGenerator:
    def __init__(self):
        global font
        self.obstacle_elapsed_time = 0
        self.obstacle_generate_time = 2
        self.gap_time = 0
        self.delay_time = 2
        self.object_list = []
        self.item_list = []
        self.level_time = 0
        self.levelup_time = 2
        self.state = REST
        self.minlevel = 0
        self.bgm = load_music('./Resources/music/bgm_game.wav')
        self.bgm.set_volume(64)
        self.bgm.repeat_play()
        font = load_font('PressStart2P.ttf', 30)

    def generate_obstacle(self):
            case = random.randint(self.minlevel, level)
            print(case)
            if case == 0:
                j = 0
                while j < (level // 3) + 1:
                    spear = Spear()
                    spear.set_obsgap(70 * j) # spear width
                    j += 1
                    self.object_list.append(spear)
            elif case == 1:
                j = 0
                while j < (level // 5) + 1:
                    thorn = Thorn()
                    thorn.set_obsgap(300 * j)
                    j += 1
                    self.object_list.append(thorn)

            elif (case >= 2) and (case < 5):
                thorn = Thorn()
                self.object_list.append(thorn)
                spear = Spear()
                spear.set_obsgap(300)
                self.object_list.append(spear)
                thorn2 = Thorn()
                thorn2.set_obsgap(600)
                self.object_list.append(thorn2)
                quick = FastRun()
                quick.set_obsgap(1200)
                self.item_list.append(quick)

            elif (case >= 5) and (case < 9):
                spear = Spear()
                self.object_list.append(spear)
                thorn = Thorn()
                thorn.set_obsgap(300)
                self.object_list.append(thorn)
                spear2 = Spear()
                spear2.set_obsgap(600)
                self.object_list.append(spear2)
                recover = Recover()
                recover.set_obsgap(900)
                self.item_list.append(recover)


            elif (case >= 9) and (case <= 10):
                spear = Spear()
                self.object_list.append(spear)
                thorn = Thorn()
                thorn.set_obsgap(300)
                self.object_list.append(thorn)
                spear2 = Spear()
                spear2.set_obsgap(600)
                self.object_list.append(spear2)
                thorn2 = Thorn()
                thorn2.set_obsgap(900)
                self.object_list.append(thorn2)
                recover = Recover()
                recover.set_obsgap(500)
                self.item_list.append(recover)


            elif (case >= 11) and (case <= 12):
                spear = Spear2()
                self.object_list.append(spear)
                thorn = Thorn2()
                thorn.set_obsgap(350)
                self.object_list.append(thorn)
                spear2 = Spear2()
                spear2.set_obsgap(700)
                self.object_list.append(spear2)
                thorn2 = Thorn2()
                thorn2.set_obsgap(1050)
                self.object_list.append(thorn2)
                giant = Giant()
                giant.set_obsgap(1300)
                self.item_list.append(giant)

            elif (case >= 13) and (case <= 14):
                thorn = Thorn2()
                self.object_list.append(thorn)
                thorn2 = Thorn2()
                thorn2.set_obsgap(350)
                self.object_list.append(thorn2)
                spear1 = Spear2()
                spear1.set_obsgap(700)
                self.object_list.append(spear1)
                thorn3 = Thorn2()
                thorn3.set_obsgap(1050)
                self.object_list.append(thorn3)
                thorn4 = Thorn4()
                thorn4.set_obsgap(1400)
                self.object_list.append(thorn4)

            elif (case >= 15) and (case <= 16):
                thorn = Thorn2()
                self.object_list.append(thorn)
                thorn2 = Thorn3()
                thorn2.set_obsgap(350)
                self.object_list.append(thorn2)
                thorn3 = Thorn2()
                thorn3.set_obsgap(700)
                self.object_list.append(thorn3)
                thorn4 = Thorn5()
                thorn4.set_obsgap(1050)
                self.object_list.append(thorn4)
                recover = Recover()
                recover.set_obsgap(500)
                self.item_list.append(recover)

            elif (case >= 17) and (case <= 18):
                spear = Spear2()
                self.object_list.append(spear)
                thorn2 = Thorn4()
                thorn2.set_obsgap(350)
                self.object_list.append(thorn2)
                thorn3 = Thorn2()
                thorn3.set_obsgap(700)
                self.object_list.append(thorn3)
                thorn4 = Thorn5()
                thorn4.set_obsgap(1050)
                self.object_list.append(thorn4)
                quick = FastRun()
                quick.set_obsgap(1400)
                self.item_list.append(quick)

            elif (case >= 19) and (case <= 20):
                spear = Spear2()
                self.object_list.append(spear)
                thorn2 = Thorn4()
                thorn2.set_obsgap(300)
                self.object_list.append(thorn2)
                thorn3 = Thorn2()
                thorn3.set_obsgap(800)
                self.object_list.append(thorn3)
                spear2 = Spear2()
                spear3 = Spear2()
                spear2.set_obsgap(1100)
                spear3.set_obsgap(1200)
                self.object_list.append(spear2)
                self.object_list.append(spear3)
                thorn4 = Thorn5()
                thorn4.set_obsgap(1600)
                self.object_list.append(thorn4)

            elif case >= 21:
                spear = Spear3()
                self.object_list.append(spear)
                thorn1 = Stage3Thorn2()
                thorn1.set_obsgap(400)
                self.object_list.append(thorn1)
            elif case >= 22:
                thorn1 = Stage3Thorn2()
                self.object_list.append(thorn1)
                spear = Spear3()
                spear.set_obsgap(400)
                self.object_list.append(spear)
                spear2 = Spear3()
                spear2.set_obsgap(500)
                self.object_list.append(spear2)
                spear3 = Spear3()
                spear3.set_obsgap(600)
                self.object_list.append(spear3)
                giant = Giant()
                giant.set_obsgap(1300)
                self.item_list.append(giant)

            elif case >= 23 and case <= 24:
                thorn1 = Stage3Thorn3()
                self.object_list.append(thorn1)
                thorn2 = Stage3Thorn4()
                thorn2.set_obsgap(300)
                self.object_list.append(thorn1)
                spear = Spear3()
                spear.set_obsgap(600)
                self.object_list.append(spear)
                thorn3 = Stage3Thorn1()
                thorn3.set_obsgap(1000)
                self.object_list.append(thorn3)
            elif case >= 25 and case <= 26:
                spear = Spear3()
                self.object_list.append(spear)
                thorn1 = Stage3Thorn3()
                thorn1.set_obsgap(300)
                self.object_list.append(thorn1)
                thorn2 = Stage3Thorn4()
                thorn2.set_obsgap(600)
                self.object_list.append(thorn1)
                spear = Spear3()
                spear.set_obsgap(900)
                self.object_list.append(spear)
                thorn3 = Stage3Thorn1()
                thorn3.set_obsgap(1300)
                self.object_list.append(thorn3)
            elif case >= 27:
                spear = Spear3()
                self.object_list.append(spear)
                spear1 = Spear3()
                spear1.set_obsgap(100)
                self.object_list.append(spear1)
                spear2 = Spear3()
                spear2.set_obsgap(200)
                self.object_list.append(spear2)
                thorn = Stage3Thorn4()
                thorn.set_obsgap(600)
                self.object_list.append(thorn)
                thorn1 = Stage3Thorn2()
                thorn1.set_obsgap(1000)
                self.object_list.append(thorn1)
                recover = Recover()
                recover.set_obsgap(500)
                self.item_list.append(recover)

    def level_check(self):
        global level, OBSTACLE_SPEED_PPS, SLOW_SPEED_PPS, BACKGROUND_SPEED_PPS, QUICK_SPEED_PPS
        OBSTACLE_SPEED_PPS = (OBSTACLE_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level
        QUICK_SPEED_PPS = (QUICK_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level

        SLOW_SPEED_PPS = (SLOW_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level * 0.5
        BACKGROUND_SPEED_PPS = (BACKGROUND_SPEED_MPS * PIXEL_PER_METER) + LEVEL_SPEED_PPS * level * 0.1
        if self.level_time >= self.levelup_time:
            self.level_time = 0
            level += 1
            self.levelup_time = level * 0.5
            print(level)
        if level > 10:
            self.minlevel = 5
        elif level > 15:
            self.minlevel = 10
        elif level > 20:
            self.minlevel = 15


    def update(self, frame_time):
        if self.state == REST:
            self.gap_time += frame_time
        elif self.state == GENERATE:
            self.obstacle_elapsed_time += frame_time

        self.level_time += frame_time
        self.level_check()
        if(self.obstacle_elapsed_time >= self.obstacle_generate_time):
            self.generate_obstacle()
            self.state = REST
            self.obstacle_elapsed_time = 0
        elif(self.gap_time >= self.delay_time):
            self.state = GENERATE
            self.gap_time = 0


    def draw(self):
        global font, speed
        font.draw(50, 750, 'level: %d' % level, (0, 0, 0))
        font.draw(50, 650, 'current speed : %f m/s' % speed, (1, 1, 1))


class Spear:
    image = None

    def __init__(self):
        self.x, self.y = 800, 530
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Spear.image == None:
            Spear.image = load_image('./Resources/image/obstacle/Stage_Spear.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del(self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass

    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        global font
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 30, self.y - 270, self.x + 30, self.y + 270

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Spear2:
    image = None

    def __init__(self):
        self.x, self.y = 800, 530
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Spear2.image == None:
            Spear2.image = load_image('./Resources/image/obstacle/Stage2_Spear.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass

    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        global font
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 70, self.y - 270, self.x + 70, self.y + 270

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Spear3:
    image = None

    def __init__(self):
        self.x, self.y = 800, 530
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Spear3.image == None:
            Spear3.image = load_image('./Resources/image/obstacle/Stage3_Spear.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        global font
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 35, self.y - 270, self.x + 35, self.y + 270

    def draw_bb(self):
        draw_rectangle(*self.get_bb())



class Thorn:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Thorn.image == None:
            Thorn.image = load_image('./Resources/image/obstacle/Stage_thorn.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x > 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 20, self.y - 25, self.x + 20, self.y + 25

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Thorn2:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Thorn2.image == None:
            Thorn2.image = load_image('./Resources/image/obstacle/Stage2_thorn.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 19, self.y - 25, self.x + 19, self.y + 25

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Thorn3:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Thorn3.image == None:
            Thorn3.image = load_image('./Resources/image/obstacle/Stage2_thorn2.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 19, self.y - 25, self.x + 19, self.y + 25

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Thorn4:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Thorn4.image == None:
            Thorn4.image = load_image('./Resources/image/obstacle/Stage2_thorn3.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 21, self.y - 49, self.x + 21, self.y + 49

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Thorn5:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Thorn5.image == None:
            Thorn5.image = load_image('./Resources/image/obstacle/Stage2_thorn4.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE


    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 21, self.y - 49, self.x + 21, self.y + 49

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Stage3Thorn1:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Stage3Thorn1.image == None:
            Stage3Thorn1.image = load_image('./Resources/image/obstacle/Stage2_thorn1.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 32, self.y - 65, self.x + 32, self.y + 65

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Stage3Thorn2:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Stage3Thorn2.image == None:
            Stage3Thorn2.image = load_image('./Resources/image/obstacle/Stage2_thorn2.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE


    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 55, self.y - 95, self.x + 55, self.y + 95
    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Stage3Thorn3:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Stage3Thorn3.image == None:
            Stage3Thorn3.image = load_image('./Resources/image/obstacle/Stage3_thorn3.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE


    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 25, self.y - 25, self.x + 25, self.y + 25

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Stage3Thorn4:
    image = None

    def __init__(self):
        self.x, self.y = 800, 230
        self.speed = SLOW_SPEED_PPS
        self.state = REST
        self.angle = 0
        if Stage3Thorn4.image == None:
            Stage3Thorn4.image = load_image('./Resources/image/obstacle/Stage3_thorn4.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        if self.state == GENERATE:
            self.x += distance * 2 # 레벨당 속도 넣으면 됨
            self.angle += 15
            if self.x >= 900:
                del (self)

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        if self.state == REST:
            self.image.draw(self.x, self.y)
        elif self.state == GENERATE:
            self.image.rotate_draw(self.angle, self.x, self.y)

    def get_bb(self):
        return self.x - 32, self.y - 50, self.x + 32, self.y + 50

    def draw_bb(self):
        draw_rectangle(*self.get_bb())


class Grass:
    def __init__(self):
        self.image1 = load_image('./Resources/image/map/First_ground.png')
        self.image2 = load_image('./Resources/image/map/Second_ground.png')

        self.image3 = load_image('./Resources/image/map/Third_ground.png')

        self.x = 0
        self.speed = SLOW_SPEED_PPS

    def draw(self):
        if level <= 10:
            self.image1.draw(750-self.x, 400)
        elif level > 10 and level <= 20:
            self.image2.draw(750 - self.x, 400)
        elif level > 20 and level <= 30:
            self.image3.draw(750 - self.x, 400)

    def update(self, frame_time):
        distance = self.speed * frame_time
        self.x += distance  # 레벨당 속도 넣으면 됨
        if(self.x >= 653.2):
            self.x = 93.75

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_quickspeed(self):
        self.speed = QUICK_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass


    def get_bb(self):
        return 0, 0, 1500, 800

    def draw_bb(self):
        draw_rectangle(*self.get_bb())
    # fill here


class Background:
    def __init__(self):
        self.image1 = load_image('./Resources/image/map/First_Background.png')
        self.image2 = load_image('./Resources/image/map/Second_Background.png')
        self.image3 = load_image('./Resources/image/map/Third_Background.png')

        self.x = 0
        self.speed = BACKGROUND_SPEED_PPS

    def draw(self):
        if level <= 10:
            self.image1.draw(750 - self.x, 400)
            self.image1.draw(2250 - self.x, 400)
        elif level > 10 and level <=20:
            self.image2.draw(750 - self.x, 400)
            self.image2.draw(2250 - self.x, 400)
        elif level > 20 and level <=30:
            self.image3.draw(750 - self.x, 400)
            self.image3.draw(2250 - self.x, 400)

    def eat(self):
        self.state = GENERATE

    def set_quickspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass
    def set_slowspeed(self):
        self.speed = BACKGROUND_SPEED_PPS
        pass

    def update(self, frame_time):
        distance = self.speed * frame_time
        self.x += distance  # 레벨당 속도 넣으면 됨
        if (self.x >= 1500):
            self.x = 0



class Giant:
    image = None

    def __init__(self):
        self.x, self.y = 800, 400
        self.speed = SLOW_SPEED_PPS
        self.type = 'Giant'
        self.state = REST
        if Giant.image == None:
            Giant.image = load_image('./Resources/image/item/giant.png')

    def set_obsgap(self, gap):
        self.x += gap


    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨
        elif self.state == GENERATE:
            self.x = -500

    def eat(self):
        self.state = GENERATE


    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def draw(self):
        global font
        if self.state == REST:
            self.image.draw(self.x, self.y)

    def get_bb(self):
        return self.x - 30, self.y - 30, self.x + 30, self.y + 30

    def draw_bb(self):
        draw_rectangle(*self.get_bb())



class Recover:
    image = None

    def __init__(self):
        self.x, self.y = 800, 400
        self.speed = SLOW_SPEED_PPS
        self.type = 'Recover'
        self.state = REST
        if Recover.image == None:
            Recover.image = load_image('./Resources/image/item/recover.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def remove(self):
        pass

    def draw(self):
        global font
        self.image.draw(self.x, self.y)

    def get_bb(self):
        return self.x - 30, self.y - 30, self.x + 30, self.y + 30

    def draw_bb(self):
        draw_rectangle(*self.get_bb())




class FastRun:
    image = None

    def __init__(self):
        self.x, self.y = 800, 400
        self.speed = SLOW_SPEED_PPS
        self.type = 'FastRun'
        self.state = REST
        if FastRun.image == None:
            FastRun.image = load_image('./Resources/image/item/fastrun.png')

    def set_obsgap(self, gap):
        self.x += gap

    def eat(self):
        self.state = GENERATE

    def update(self, frame_time):
        global speed
        speed = self.speed / PIXEL_PER_METER
        distance = self.speed * frame_time
        if self.state == REST:
            self.x -= distance # 레벨당 속도 넣으면 됨

    def set_fastspeed(self):
        self.speed = OBSTACLE_SPEED_PPS
        pass

    def set_slowspeed(self):
        self.speed = SLOW_SPEED_PPS
        pass

    def remove(self):
        pass

    def draw(self):
        global font
        self.image.draw(self.x, self.y)

    def get_bb(self):
        return self.x - 30, self.y - 30, self.x + 30, self.y + 30

    def draw_bb(self):
        draw_rectangle(*self.get_bb())
