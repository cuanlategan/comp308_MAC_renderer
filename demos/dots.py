#! /usr/bin/python2

from sample import Sampler
from math import pi
import cairo

R = 25

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 400, 400)
ctx = cairo.Context(surface)
ctx.set_source_rgb(1,1,1)
ctx.paint()
ctx.set_source_rgb(0,0,0)

sample = Sampler(float(R)/400)
for i in range(200):
    sample._add(sample.generate())

#COLOURS = [(1,0,0), (1,1,0), (0,0,0), (0,1,0), (0,1,1)]
for i, dot in enumerate(sample):
    #ctx.set_source_rgb(COLOURS[i][0], COLOURS[i][1], COLOURS[i][2])
    ctx.arc(dot.x*200+200, dot.y*200+200,  2, 0, 2*pi)
    ctx.fill()
    #for scollad in dot.boundary:
    #    for x,y in scollad.path():
    #        ctx.line_to(x*200+200, y*200+200)
    #    ctx.stroke()


surface.write_to_png("out.png")
