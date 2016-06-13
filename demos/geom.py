from math import *
import sys
epsilon = sys.float_info.epsilon
from random import random

MAX_PER_CELL = 9
MIN_AREA = 0.00000001

class Disc(object):
    def __init__(self, pt, r):
        self.x, self.y = self.P = pt
        self.r = r

        self.boundary = [Scollad(pt, 0, 2*pi, pt, r*2, 1, pt, r*3, 1)]
        self.area = self.boundary[0].area

    def __str__(self):
        return str(self.P)

    __repr__ = __str__

    def generate(self):
        # Scale probabilities by area
        a = random()*self.area
        for scollad in self.boundary:
            if a < scollad.area:
                return Disc(scollad.generate(), self.r)
            a -= scollad.area
        return None

    def clip(self, disc, scale=2):
        self.area = 0
        newBoundary = []
        r, disc.r = disc.r, disc.r*scale

        for scollad in self.boundary:
            last = None
            for sub in scollad.clip(disc):
                if sub.area > MIN_AREA:
                    self.area += sub.area
                    if last and (last.a2 == sub.a1 and last.arcs[0] == sub.arcs[0]
                            and last.arcs[1] == sub.arcs[1]):
                        last.a2 = sub.a2
                        last.area = last.calcAreaToAngle(last.a2)
                        continue
                    last = sub
                    newBoundary.append(sub)

        disc.r = r
        self.boundary = newBoundary

def integralOfDistToCircle(x, arc):
    if arc.r < epsilon:
        return 0
    
    sin_x = sin(x)
    dsin_x = arc.d*sin_x
    y = sin_x*arc.d/arc.r
    if y < -1: y = -1
    elif y > 1: y = 1
    theta = asin(y)

    k = arc.sign
    r = arc.r
    theta = arc.theta
    d = arc.d

    return (r*(r*(x + k*theta) + k*cos(theta)*dsin_x) + d*cos(x)*dsin_x)*0.5

class Scollad(object):
    def __init__(self, pt, a1, a2, P1, r1, sign1, P2, r2, sign2):
        self.x, self.y = self.P = pt
        self.a1, self.a2 = a1, a2
        self.arcs = Arc(P1, r1, sign1, self, a1), Arc(P2, r2, sign2, self, a1)

        self.area = self.calcAreaToAngle(a2)

    def __str__(self):
        return "Scollad({}, {}, {}, arcs={}, area={})".format(self.P, self.a1, self.a2, self.arcs, self.area)

    __repr__ = __str__

    def calcAreaToAngle(self, angle):
        underInner = integralOfDistToCircle(angle - self.arcs[0].theta,
                                self.arcs[0]) - self.arcs[0].integralAtStart
        underOuter = integralOfDistToCircle(angle - self.arcs[1].theta,
                                self.arcs[1]) - self.arcs[1].integralAtStart

        return underOuter - underInner

    def path(self):
        def angleToPoint(angle, arc):
            arc = self.arcs[arc]
            a = angle - arc.theta
            dist = arc.d*cos(a) + arc.sign*sqrt(abs(arc.r2 - (arc.d*sin(a))**2))
            return cos(angle)*dist + self.x, sin(angle)*dist + self.y

        def xrange(start, end, inc):
            counter = start
            while (counter < end and inc > 0
                    ) or (counter > end and inc < 0):
                yield counter
                counter += inc

        ret = [angleToPoint(self.a1, 0)]
        s = 0.01 if self.a2 > self.a1 else -0.01
        for angle in xrange(self.a1, self.a2, s):
            ret.append(angleToPoint(angle, 1))
        for angle in xrange(self.a2, self.a1, -s):
            ret.append(angleToPoint(angle, 0))

        return ret

    def _distToCurve(self, angle, index):
        a = angle - self.arcs[index].theta
        sin_a = sin(a)
        t0 = self.arcs[index].r2 - self.arcs[index].d2*(sin_a**2)

        if t0 < 0:
            return self.arcs[index].d*cos(a)
        else:
            return (self.arcs[index].d*cos(a) +
                    self.arcs[index].sign*sqrt(t0))

    def generate(self):
        def calcAngleForArea(area):
            lo, hi = self.a1, self.a2
            cur = lo + (hi-lo)*random()

            for i in range(10):
                if self.calcAreaToAngle(cur) < area:
                    lo = cur
                    cur = (cur + hi)*0.5
                else:
                    hi = cur
                    cur = (lo + cur)*0.5

            return cur

        angle = calcAngleForArea(self.area*random())
        d1 = self._distToCurve(angle, 0)
        d2 = self._distToCurve(angle, 1)
        d = sqrt(d1**2 + (d2**2 - d1**2)*random())

        return (self.x + cos(angle)*d, self.y + sin(angle)*d)

    def clip(self, disc):
        def canonicalize(angle):
            delta = (angle - self.a1) % (2*pi)
            if delta < 0: delta += 2*pi
            return self.a1 + delta

        def distToCircle(angle, disc):
            vx,vy = (disc.x-self.x, disc.y-self.y)
            d2 = vx**2 + vy**2
            theta = atan2(vy, vx)
            alpha = angle-theta
            sin_alpha = sin(alpha)
            x2 = disc.r**2 - d2*(sin_alpha**2)

            if x2 < 0:
                return -10000000000, -10000000000
            else:
                a = sqrt(d2)*cos(alpha)
                x = sqrt(x2)
                return a-x, a+x

        angles = []
        vx,vy = (disc.x - self.x, disc.y - self.y)
        d = sqrt(vx**2 + vy**2)

        # Is the scollap's center within the disc?
        if disc.r < d:
            theta = atan2(vy, vx)
            x = sqrt(d**2 - disc.r**2)
            alpha = asin(disc.r/d)
            angle = canonicalize(theta+alpha)

            if self.a1 < angle < self.a2:
                if self._distToCurve(angle, 0) < x < self._distToCurve(angle, 1):
                    angles.append(angle)
            angle = canonicalize(theta-alpha)
            if self.a1 < angle < self.a2:
                if self._distToCurve(angle, 0) < x < self._distToCurve(angle, 1):
                    angles.append(angle)

        for arc in self.arcs:
            vx,vy = (disc.x - arc.x, disc.y - arc.y)
            d = sqrt(vx**2 + vy**2)

            if d > epsilon:
                invd = 1.0/d
                x = (d**2 - disc.r**2 + arc.r**2)*invd*0.5
                k = arc.r**2 - x**2

                if k > 0:
                    y = sqrt(k)
                    vx *= invd
                    vy *= invd
                    vx_x,vy_x = vx*x,vy*x
                    vx_y,vy_y = vx*y,vy*y

                    angle = canonicalize(atan2(arc.y + vy_x + vx_y - self.y,
                                                arc.x + vx_x - vy_y - self.x))
                    if self.a1 < angle < self.a2:
                        angles.append(angle)
                    angle = canonicalize(atan2(arc.y + vy_x - vx_y - self.y,
                                                arc.x + vx_x + vy_y - self.x))
                    if self.a1 < angle < self.a2:
                        angles.append(angle)

        angles.sort()
        angles.append(self.a2)

        a1 = self.a1
        for a2 in angles:
            def derive(arc0, arc1):
                return Scollad(self.P, a1, a2,
                                arc0.P, arc0.r, getattr(arc0, 'sign', 1),
                                arc1.P, arc1.r, getattr(arc1, 'sign', -1))

            midA = (a1+a2)*0.5
            inner = self._distToCurve(midA, 0)
            outer = self._distToCurve(midA, 1)
            d1, d2 = distToCircle(midA, disc)

            if d2 < inner or d1 > outer:
                yield derive(self.arcs[0], self.arcs[1])
            else:
                if inner < d1:
                    yield derive(self.arcs[0], disc)
                if d2 < outer:
                    yield derive(disc, self.arcs[1])

            a1 = a2

class Arc(object):
    def __init__(self, P, r, sign, pt=None, a=0):
        if pt is None: pt = self

        self.x, self.y = self.P = P
        self.r = r
        self.sign = sign

        # Cached computations
        vx, vy = (self.x-pt.x, self.y-pt.y)
        self.d2 = vx**2 + vy**2
        self.d = sqrt(self.d2)
        self.r2 = self.r**2
        self.theta = atan2(vy, vx)
        self.integralAtStart = integralOfDistToCircle(a - self.theta, self)

    def __str__(self):
        return "Arc({}, {}, {})".format(self.P, self.r, self.sign)

    __repr__ = __str__

    def __eq__(self, other):
        return isinstance(other, Arc) and (
            self.P == other.P and self.r == other.r and self.sign == other.sign)
