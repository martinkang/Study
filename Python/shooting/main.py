import pygame
import random
from time import sleep

BLACK = ( 0, 0, 0 )
RED = ( 255, 0, 0 )

PAD_WIDTH = 480
PAD_HEIGHT = 640
FIGHTER_WIDTH = 36
FIGHTER_HEIGHT = 38
FIGHTER_SPEED = 0.2
ENEMY_WIDTH = 26
ENEMY_HEIGHT = 20
ENEMY_SPEED = 0.3

def drawObject( aObj, x, y ):
    global gGamePad
    gGamePad.blit( aObj, ( x, y ) )

def displayMsg( aMsg ):
    global gGamePad

    sTxt = aMsg
    
    sTxtFont = pygame.font.Font( 'freesansbold.ttf', 80 )
    sTxt = sTxtFont.render( sTxt, True, RED )
    sTxtPos = sTxt.get_rect()
    sTxtPos.center = ( PAD_WIDTH / 2 , PAD_HEIGHT / 2 )
    
    gGamePad.blit( sTxt, sTxtPos )
    pygame.display.update()
    
    
def isHit( x, y, e_x, e_y ):
    if ( y >= e_y and y <= e_y + ENEMY_HEIGHT ) or \
       ( y + FIGHTER_HEIGHT >= e_y and y+ FIGHTER_HEIGHT <= e_y + ENEMY_HEIGHT ) or \
       ( y <= e_y and y + FIGHTER_HEIGHT >= e_y + ENEMY_HEIGHT ):  
        if ( x >= e_x and x <= e_x + ENEMY_WIDTH ) or \
           ( x + FIGHTER_WIDTH >= e_x and x + FIGHTER_WIDTH < e_x + ENEMY_WIDTH ) or \
           ( x <= e_x and x + FIGHTER_WIDTH >= e_x + ENEMY_WIDTH ):           
                return True
    
    return False

def runEvent( x, y, e_x, e_y ):
    x_change = 0
    y_change = 0

    sDoneFlag = False

    while not sDoneFlag:
        e_y += ENEMY_SPEED
        if e_y > PAD_HEIGHT:
            e_y = 0
            e_x = random.randrange( 0, PAD_WIDTH - ENEMY_WIDTH )
            
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sDoneFlag = True

            if event.type == pygame.KEYDOWN: 
                if event.key == pygame.K_LEFT:
                    x_change -= FIGHTER_SPEED
                elif event.key == pygame.K_RIGHT:
                    x_change += FIGHTER_SPEED
                elif event.key == pygame.K_UP:
                    y_change -= FIGHTER_SPEED
                elif event.key == pygame.K_DOWN:
                    y_change += FIGHTER_SPEED
                elif event.key == pygame.K_ESCAPE:
                    sDoneFlag = True
                    break

            if event.type == pygame.KEYUP:
                if event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                    x_change = 0
                elif event.key == pygame.K_UP or event.key == pygame.K_DOWN:
                    y_change = 0

        x += x_change
        y += y_change
        if x < 0:
            x = 0
        elif x > PAD_WIDTH - FIGHTER_WIDTH:
            x = PAD_WIDTH - FIGHTER_WIDTH

        if y < 0:
            y = 0
        elif y > PAD_HEIGHT - FIGHTER_HEIGHT:
            y = PAD_HEIGHT - FIGHTER_HEIGHT

        if isHit( x, y, e_x, e_y ) == True:
            sDoneFlag = True
            displayMsg( "Game Over" )
            break
        
        gGamePad.fill( BLACK )
        drawObject( gFighter, x, y )
        drawObject( gEnemy, e_x, e_y )
        pygame.display.update()

def runGame():
    global gGamePad, gClock, gFighter, gEnemy

    gGamePad.fill( BLACK )

    # 초기 위치 설정
    x = PAD_WIDTH * 0.45
    y = PAD_HEIGHT * 0.9

    enemy_x = random.randrange( 0, PAD_WIDTH - ENEMY_WIDTH )
    enemy_y = 0
    
    drawObject( gFighter, x, y )
    drawObject( gEnemy, enemy_x, enemy_y )
    pygame.display.update()
    # 게임의 초당 프레임 설정
    gClock.tick( 60 )

    runEvent( x, y, enemy_x, enemy_y )

    pygame.quit()

def initGame():
    global gGamePad, gClock, gFighter, gEnemy

    # pygame 라이브러리 초기화
    pygame.init()

    gGamePad = pygame.display.set_mode( ( PAD_WIDTH, PAD_HEIGHT ) )
    pygame.display.set_caption( "Game" )
    gFighter = pygame.image.load( 'img/fighter.png' )
    gEnemy = pygame.image.load( 'img/enemy.png' )
    gClock = pygame.time.Clock()

initGame()
runGame()
