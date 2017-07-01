import pygame

BLACK = ( 0, 0, 0 )
PAD_WIDTH = 480
PAD_HEIGHT = 640
FIGHTER_WITH = 36
FIGHTER_HEIGHT = 38

def drawObject( aObj, x, y ):
    global gGamePad
    gGamePad.blit( aObj, ( x, y ) )
    
def runEvent( x, y ):
    x_change = 0
    y_change = 0

    sDoneFlag = False

    while not sDoneFlag:
        for event in pygame.event.get():
            if event.type == pygame.K_ESCAPE:
                sDoneFlag = True

            if event.type == pygame.KEYDOWN: 
                if event.key == pygame.K_LEFT:
                    x_change -= 0.2
                elif event.key == pygame.K_RIGHT:
                    x_change += 0.2
                elif event.key == pygame.K_UP:
                    y_change -= 0.2
                elif event.key == pygame.K_DOWN:
                    y_change += 0.2

            if event.type == pygame.KEYUP:
                if event.key == pygame.K_LEFT or \
                event.key == pygame.K_RIGHT or \
                event.key == pygame.K_UP or \
                event.key == pygame.K_DOWN:
                    x_change = 0
                    y_change = 0

        x += x_change
        y += y_change
        if x < 0:
            x = 0
        elif x > PAD_WIDTH - FIGHTER_WITH:
            x = PAD_WIDTH - FIGHTER_WITH

        if y < 0:
            y = 0
        elif y > PAD_HEIGHT - FIGHTER_HEIGHT:
            y = PAD_HEIGHT - FIGHTER_HEIGHT

        gGamePad.fill( BLACK )
        drawObject( gFighter, x, y )
        pygame.display.update()

def runGame():
    global gGamePad, gClock, gFighter

    gGamePad.fill( BLACK )

    # 초기 위치 설정
    x = PAD_WIDTH * 0.45
    y = PAD_HEIGHT * 0.9

    drawObject( gFighter, x, y )
    pygame.display.update()
    # 게임의 초당 프레임 설정
    gClock.tick( 60 )

    runEvent( x, y )

    pygame.quit()

def initGame():
    global gGamePad, gClock, gFighter

    # pygame 라이브러리 초기화
    pygame.init()

    gGamePad = pygame.display.set_mode( ( PAD_WIDTH, PAD_HEIGHT ) )
    pygame.display.set_caption( "Game" )
    gFighter = pygame.image.load( 'img/fighter.png' )
    gClock = pygame.time.Clock()

initGame()
runGame()
