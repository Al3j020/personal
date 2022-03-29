sub init()
    fieldsToAdd = {
        firstName: "Alejandro"
        lastName: "Gutiérrez"
        calcAge : function(birthYear)
            m.age = 2022 - birthYear
            print substitute("{0} {1} was born in {2}, so he is {3} years old", m.firstName, m.lastName, birthYear.toStr(), m.age.toStr())
        end function
    }
    fieldsToAdd.calcAge(1890)

    calcAge = function(birthYear)
        print 2022 - birthYear
        return 2022 - birthYear
    end function

    nodeToAdd = {
        utils: createObject("roSGNode","util")
    }

    globalAA = getGlobalAA()
    if globalAA.calcAge = invalid
        globalAA.calcAge = calcAge
        globalAA.util = nodeToAdd
    end if

    ?"{mainScene}", globalAA, globalAA.calcAge(1997)


    m.global.addFields(nodeToAdd)
    utils = m.global.utils
    utils.callFunc("logger", {param: "params"})
end sub