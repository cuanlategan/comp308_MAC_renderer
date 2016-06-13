from geom import *
from math import *
from random import random, randrange

class Sampler(object):
    def __init__(self, radius, origin=-1, width=2, prestart=True):
        self.radius = radius
        self.origin = origin
        self.width = width

        # choose grid_size so that 4r search only requires checking adjacent cells
        # also determines max points per cell
        self.grid_size = int(ceil(2.0/(4.0*radius)))
        if self.grid_size < 2: self.grid_size = 2
        self.grid = [[[] for i in range(self.grid_size)]
                        for j in range(self.grid_size)]
        self.candidates = []

        if prestart:
            self._add(Disc((width*random()+origin, 2*random()+origin), radius))

    def __iter__(self):
        for column in self.grid:
            for cell in column:
                for disc in cell:
                    yield disc

    def _add(self, disc):
        if disc is None: return
        def eachPoint():
            MAX = self.grid_size - 1
            for x in range(max(gx-2, 0), min(gx+2, MAX)):
                for y in range(max(gy-2, 0), min(gy+2, MAX)):
                    for cell in self.grid[x][y]:
                        yield cell

        def isValid():
            if (disc.x < self.origin or disc.x > self.origin + self.width) or (
                    disc.y < self.origin or disc.y > self.origin + self.width):
                return False
            for p in eachPoint():
                if (disc.x-p.x)**2 + (disc.y-p.y)**2 <= p.r**2:
                    #print "FAILURE! The randomly placed circle in an invalid spot"
                    return False
            return True

        def updateScallops():
            for p in eachPoint():
                p.clip(disc)
                disc.clip(p, 3)
                if p.area:
                    self.candidates.append(p)

        gx, gy, cell = self._cell(disc)
        updateScallops()
        if cell is None or not isValid(): return

        cell.append(disc)
        if len(cell) > MAX_PER_CELL:
            #print "FAILURE! Tried to add item to full cell"
            return #raise AssertionError
        self.candidates.append(disc)

    def _remove(self, disc):
        def eachPoint():
            MAX = self.grid_size - 1
            for x in range(max(gx-1, 0), min(gx+1, MAX)):
                for y in range(max(gy-1, 0), min(gy+1, MAX)):
                    for cell in self.grid[gx][gy]:
                        yield cell

        def rebuildScallops():
            points = list(eachPoint())
            for i, p in enumerate(points):
                p.rebuildScallops(points[:i])
                self.candidates.append(p)

        gx, gy, cell = self._cell(disc)
        if not cell: return
        cell.remove(disc)
        rebuildScallops()

    def neighbours(self, disc):
        MAX = self.grid_size - 1
        gx, gy = self._grid(disc)
        for x in range(max(gx-1, 0), min(gx+1, MAX)):
            for y in range(max(gy-1, 0), min(gy+1, MAX)):
                for cell in self.grid[gx][gy]:
                    delta = cell.x - disc.x, cell.y - disc.y
                    dist2 = delta[0]*delta[0] + delta[1]*delta[1]
                    yield cell

    def _grid(self, pt):
        return (int(((pt.x-self.origin)/self.width)*self.grid_size),
                int(((pt.y-self.origin)/self.width)*self.grid_size))

    def _cell(self, pt):
        gx, gy = self._grid(pt)
        if 0 <= gx < self.grid_size and 0 <= gy < self.grid_size:
            return gx, gy, self.grid[gx][gy]
        return gx, gy, None


    def generate(self):
        while self.candidates:
            candidate = self.candidates.pop(randrange(len(self.candidates)))
            point = candidate.generate()
            if point is not None:
                return point

    def fill(self):
        p = self.generate()
        while p:
            self._add(p)
            p = self.generate()

        # Quick fix
        if len(list(self)) < 10:
            self.candidates = list(self)
            self.fill()
