#! /usr/bin/python2
import pygame
pygame.init()
from pygame.locals import *

from sample import Sampler
import geom

R = 25
AQUA = (0,255,255)
RED = (255,0,0)
screen = pygame.display.set_mode((400,400))
pygame.display.set_caption("Be like water")
background = pygame.Surface(screen.get_size())
background = background.convert()
background.fill((0,0,0))

pygame.draw.rect(background, RED, (300, 170, 100, 60))
pygame.draw.rect(background, RED, (0, 0, 100, 100))
pygame.draw.rect(background, RED, (0, 300, 100, 100))

screen.blit(background, (0,0))
pygame.display.flip()

class vec2(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

def render(sample):
    """ Basic algorithm for rendering the water texture."""
    # TODO Implement in OpenGL for speed,
    #   catch: How do I format texture strings to pass the sample data to GLSL?
    #   Maybe that's what I get for not using C++. 
    for x in range(400):
        for y in range(400):
            samplePoint = vec2(x, y)
            gx, gy = sample._grid(samplePoint)
            if not sample.grid[gx][gy]: continue
            disc = sample.grid[gx][gy][0]

            out = [0,0,0]
            count = 0
            for neighbour in sample.neighbours(disc):
                textPos = samplePoint.x - neighbour.x, samplePoint.y - neighbour.y
                textPos = (int(textPos[0] + waterRect.centerx),
                            int(textPos[1] + waterRect.centery))
                if (textPos[0] < 0 or textPos[1] < 0 or
                        textPos[0] >= waterRect.width
                        or textPos[1] >= waterRect.height):
                    continue

                pixel = waterText.get_at(textPos)
                out[1] += pixel[1]; out[2] += pixel[2] # Sum, but don't set red
                count += 1
            if count:
                screen.set_at((x,y),
                        Color(min(out[0], 255), out[1]/count, out[2]/count))

def main():
    sample = Sampler(R, 0, 400)
    for i in range(120):
        sample._add(sample.generate())

    clock = pygame.time.Clock()
    while True:
        clock.tick(60)
        print "!"
        for event in pygame.event.get():
            if event.type == QUIT:
                return

        screen.blit(background, (0,0))
        # render(sample) # FIXME slow
        hasDots = False
        for dot in sample:
            if dot:
                hasDots = True
                pygame.draw.circle(screen, AQUA, (int(dot.x), int(dot.y)), 2)
        pygame.display.flip()

        # Failed to find any dots, regenerate them
        for i in range(120):
            sample._add(sample.generate())
        
        # Generate next frame of animation - TODO this isn't quite realtime yet
        newSampler = Sampler(R, 0, 400, False)
        for disc in sample:
            x, y = disc.P
            dx, dy = -8, 0
            if y > 300 and x > 200: dy = -5
            elif y < 100 and x > 200: dy = 5

            P = x, y = x+dx, y+dy
            # cull points not on the water surface. 
            if 170 < y < 230 and x > 300: continue
            if y < 100 and x < 100: continue
            if y > 300 and x < 100: continue

            newSampler._add(geom.Disc((x+dx, y+dy), float(R)))
        sample = newSampler
        for i in range(15):
            P = sample.generate()
            if P is None: continue
            if 170 < P.y < 230 and P.x > 300: continue
            if P.y < 100 and P.x < 100: continue
            if P.y > 300 and P.x < 100: continue
            print i
            sample._add(sample.generate())
        
main()
