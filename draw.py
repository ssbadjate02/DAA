import turtle

def generateSquarePoints(i):
    """ generate points for a square of any size """
    return [(i, 0), (i, i), (0, i), (0, 0)]

def drawPolyLine(edges):
    """ draw shapes using a list of coordinates """
    turtle.pencolor('cyan')
    turtle.bgcolor('pink')    
    turtle.speed(3)
    turtle.pensize(2.5)
    turtle.penup()

    turtle.goto(edges[0][0])  # make the turtle go to the start position

    turtle.pendown()
    turtle.begin_fill()
    turtle.goto(edges[0][1])
    for i in range(1,len(edges)):  # go through a list of (relative) points
        if(edges[i][0]!=edges[i-1][1]):
            turtle.end_fill
            turtle.penup()
            turtle.goto(edges[i][0])
            turtle.pendown()
            turtle.begin_fill()
        turtle.goto(edges[i][1])
        

    turtle.end_fill()
    turtle.penup()

if __name__ == "__main__":

    def testPolyLines():
        """ test shapes shape drawing functions """
        # First square
        # Shape = [(50, 0), (50, 50), (0, 50), (0, 0)]
        file = open("inputPy.txt", "r")
        edgeList = []
        maxx = 0
        maxy = 0
        minx = 10000000
        miny = 10000000

        while True:
            line = file.readline().split(' ')
            if line == ['']:
                # either end of file or just a blank line.....
                # we'll assume EOF, because we don't have a choice with the while loop!
                break
            if(line[3][-1]=='\n'): line[3] = line[3][:-1]
            if(len(line)>=4): 
                edgeList += [[[float(line[0]),(float)(line[1])],[(float)(line[2]),(float)(line[3])]]]
                maxx = max(maxx,float(line[0]),float(line[2]))
                maxy = max(maxy,float(line[1]),float(line[3]))
                minx = min(minx,float(line[0]),float(line[2]))
                miny = min(miny,float(line[1]),float(line[3]))


            # print(line)
        print(maxx," ",minx," ",maxy," ",miny)
        for i in range(len(edgeList)):
            edgeList[i][0][0] *= ((400)/(maxx-minx))
            edgeList[i][0][1] *= ((400)/(maxy-miny))
            edgeList[i][1][0] *= ((400)/(maxx-minx))
            edgeList[i][1][1] *= ((400)/(maxy-miny))

        # print(edgeList)
        drawPolyLine(edgeList)

        # Second square
        


    def main():
        testPolyLines()
        turtle.done()

    main()