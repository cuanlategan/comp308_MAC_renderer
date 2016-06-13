#! /usr/bin/python2
import pygame
from pygame.locals import *

from math import pi
import geom

BLACK = (0,0,0)
R = 25

screen = pygame.display.set_mode((400,400))
pygame.display.set_caption("Scollads! (What?)")
background = pygame.Surface(screen.get_size())
background = background.convert()
background.fill((255, 255, 255))

screen.blit(background, (0,0))
pygame.display.flip()

def toPoint((x,y)):
    return int(x), int(y)

def main():
    arc = geom.Arc((100,100), 50, 1)
    scollad = geom.Scollad((200,200), 0, 2*pi,
                            (200,200), R*2, 1, (200,200), R*3, 1)

    clock = pygame.time.Clock()
    while True:
        clock.tick(5)
        for event in pygame.event.get():
            if event.type == QUIT:
                return

        screen.blit(background, (0,0))

        pointer = geom.Scollad(pygame.mouse.get_pos(), 0, 2*pi, pygame.mouse.get_pos(), R*2, 1, pygame.mouse.get_pos(), R*3, 1)
        for sec in pointer.clip(geom.Disc(scollad.P, R*2)):
            pygame.draw.lines(screen, Color("red"), True, sec.path(), 3)

        for sec in scollad.clip(geom.Disc(pygame.mouse.get_pos(), R*3)):
            #print sec,
            pygame.draw.circle(screen, BLACK, 
                                toPoint(sec.generate()), 2)
            pygame.draw.lines(screen, BLACK, True, sec.path(), 3)
        #print
        
        pygame.display.flip()

main()

