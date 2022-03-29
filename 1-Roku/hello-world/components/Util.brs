function init ()
    ?"init >"
    ' fieldsToAdd = {
    '     firstName: "Alejandro"
    '     lastName: "Gutiérrez"
    '     logger : function(birthYear)
    '         m.age = 2022 - birthYear
    '         print substitute("{0} {1} was born in {2}, so he is {3} years old", m.firstName, m.lastName, birthYear.toStr(), m.age.toStr())
    '     end function
    ' }
    ' m.top.addFields(fieldsToAdd)
end function

function logger(params as Object) as Object
    ?"[log] >",params
    fieldsToAdd = {
        firstName: "Alejandro"
        lastName: "Gutiérrez"
        calcAge : function(birthYear)
            m.age = 2022 - birthYear
            print substitute("{0} {1} was born in {2}, so he is {3} years old", m.firstName, m.lastName, birthYear.toStr(), m.age.toStr())
        end function
    }
    m.top.addFields({newFields: fieldsToAdd})
    globalAA = getGlobalAA()
    ?"{util}",globalAA
end function