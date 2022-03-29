sub init()
    ' initSize = 500
    ' factor = 0.125
    ' opacity = 1.0
    ' pi = 3.1416 '180º
    ' initSquare = m.top.createChild("rectangle")
    ' initSquare.translation = [960, 540]
    ' initSquare.opacity = opacity
    ' initSquare.width = initSize
    ' initSquare.height = initSize
    ' initSquare.color = "#DAF7A6"
    ' initsquare.rotation = pi/8

    concentricSquares()
end sub

sub concentricSquares()
    initSize = 1080
    factor = 0.125
    opacity = 0.5
    pi = 3.1416
    rotation = 0
    initSquare = m.top.createChild("rectangle")
    initSquare.translation = [460, 0]
    initSquare.opacity = opacity
    initSquare.width = initSize
    initSquare.height = initSize
    initSquare.color = "#FFFFFF"

    newSize = initSize
    x = initSquare.translation[0]
    y = initSquare.translation[1]

    for i = 0 to 30
        tempSize = newSize
        newSize = newSize - factor*newSize
        x = x + (tempSize - newSize)/2
        y = y + (tempSize - newSize)/2
        

        square = m.top.createChild("rectangle")
        square.opacity = opacity
        square.translation = [x, y]
        square.width = newSize
        square.height = newSize

        if i MOD 2
            square.color = "#FFFFFF"
            square.rotation = pi/64
        else
            square.color = "#000000"
        end if
    
        ' rotation = rotation + pi/32
        ' ?tempSize,newSize,x,rotation
        ' square.rotation = rotation
    end for
end sub