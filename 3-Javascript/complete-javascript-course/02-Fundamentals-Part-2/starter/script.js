"use strict";
/*
let hasDriversLicense = false;
const passTest = true;

// if (passTest) hasDriverLicense = true; without strict mode this bug is untrackable
if (passTest) hasDriversLicense = true;
if  (hasDriversLicense) console.log('I can drive :D')

// const interface = 'Audio';
// const private = 546;
// const if = true;

// 33. Functions

function logger() {
    console.log('My name is Alejo');
}

// calling / running / invoking function
logger();
logger();
logger();

function fruitProcessor(apples, oranges) {
    const juice = `Juice with ${apples} apples and ${oranges} oranges`;
    return juice;
}

const appleJuice = fruitProcessor(5, 0);
console.log(appleJuice);
console.log(fruitProcessor(5, 0));

const appleOrangeJuice = fruitProcessor(2, 4);
console.log(appleOrangeJuice);

const num = Number('92')


// 34. Function Declarations vs. Expressions
// Order matters!

// Function declaration
const age1 = calcAge1(1991);

function calcAge1(birthYear) {
    return 2037 - birthYear;
}
console.log(age1);

// Function expression
const calcAge2 = function(birthYear) {
    return 2037 - birthYear;
}

const age2 = calcAge2(1994);
console.log(age2);

// Arrow function
const calcAge3 = birthYear => 2037 - birthYear;
const age3 = calcAge3(1993);
console.log(age3);

const yearsUntilRetirement = (birthYear, firstName) => {
    const age = 2021 - birthYear;
    const retirement = 63 - age;
    // return retirement;
    return `${firstName} retires in ${retirement} years`
}

console.log(yearsUntilRetirement(1990, 'Alejandro'));
console.log(yearsUntilRetirement(1993, 'Jonas'));



function cutFruitPieces(fruit) {
    return fruit * 4;
}

function fruitProcessor(apples, oranges) {
    const applePieces = cutFruitPieces(apples);
    const orangePieces = cutFruitPieces(oranges);

    const juice = `Juice with ${applePieces} apples and ${orangePieces} oranges`;
    return juice;
}

console.log(fruitProcessor(2,3));


const calcAge = function(birthYear) {
    return 2037 - birthYear;
}
const yearsUntilRetirement = function (birthYear, firstName) {
    const age = calcAge(birthYear);
    const retirement = 63 - age;

    if (retirement > 0){
        console.log(`${firstName} retires in ${retirement} years`)
        return retirement;
    } else {
        console.log(`${firstName} has already retired`)
        return -1
    }
}


const calcAverage = (a,b,c) => (a + b + c)/3;
const avgDolphins = calcAverage(44,23,71);
const avgKoalas = calcAverage(65,54,49);
const avgDolphins2 = calcAverage(85,54,41);
const avgKoalas2 = calcAverage(23,34,27);
function checkWinner(avgDolphins, avgKoalas) {
    if (avgDolphins >= 2*avgKoalas) {
        console.log(`Dolphins win 🏆 (${avgDolphins} vs ${avgKoalas})`)
    } else if (avgKoalas >= 2*avgDolphins) {
        console.log(`Koalas win 🏆 (${avgKoalas} vs ${avgDolphins})`)
    } else {
        console.log(`No team wins... 😭`)
    }
}
checkWinner(avgDolphins, avgKoalas)
checkWinner(avgDolphins2, avgKoalas2)

//Arrays

const friends = ['Michael','Steven','Peter'];
const years = new Array(1990,1991,1890,1765);
// Add Elements
const newLength = friends.push('Jay');
console.log(friends, newLength);

friends.unshift('John');
console.log(friends);

// Remove Elements
const popped = friends.pop();
console.log(popped,friends);

const shifted = friends.shift();
console.log(shifted, friends);

console.log(friends.indexOf('Michael'));
console.log(friends.indexOf('John'));

friends.push(23)
console.log(friends.includes('Michael'));
console.log(friends.includes('John'));
console.log(friends.includes('23'));
console.log(friends.includes(23));


const calcTip = function(bill) {
    return bill >= 50 && bill <=300 ? bill * 0.15: bill *0.2;
}
const bills = [125, 555, 44]
const tips = [calcTip(bills[0]), calcTip(bills[1]), calcTip(bills[2])]
const total = []

total[0] = bills[0] + tips[0]
total[1] = bills[1] + tips[1]
total[2] = bills[2] + tips[2]
console.log(total)


// Objects

const jonas = {
    firstName: "Jonas",
    lastName: "Schmedtmann",
    age: 2037 - 1991,
    job: "teacher",
    friends: ["Michael", "Peter", "Steven"]
};

const nameKey = "Name"

console.log(jonas.lastName)
console.log(jonas["first"+ nameKey])
console.log(jonas["last"+ nameKey])

// const interestedIn = prompt("What do yoy want to know? Choose between: firstName, lastName, age, job and friends")
// if (jonas[interestedIn]) {
//     console.log(jonas[interestedIn]);
// } else {
//     console.log("Wrong request!");
// }
console.log(jonas.friends.length)
console.log(`${jonas.firstName} has ${jonas.friends.length} friends, and his best friend is called ${jonas.friends[0]}`)


//Object Methods
const jonas = {
    firstName: "Jonas",
    lastName: "Schmedtmann",
    birthYear: 1991,
    job: "teacher",
    friends: ["Michael", "Peter", "Steven"],
    hasDriversLicense: true,
    // calcAge: function(birthYear) {
    //     return 2037 - birthYear;
    // }

    // calcAge: function() {
    //     console.log(this);
    //     return 2037 - this.birthYear;
    // }

    calcAge: function() {
        this.age = 2037 - this.birthYear
        return this.age;
    },
    getSummary: function() {
        return `${this.firstName} is a ${this.calcAge()} year old ${this.job}, and he has ${this.hasDriversLicense ? 'a' : 'no'} driver's license`
    }
};
console.log(jonas.age);
console.log(jonas.calcAge());
console.log(jonas.age);
console.log(jonas.getSummary());

const mark = {
    firstName: "Mark",
    mass: 78,
    height: 1.69,
}

const john = {
    firstName: "John",
    mass: 92,
    height: 1.95,
}

const calcBMI = function() {
    this.BMI = this.mass/(this.height ** 2)
    return this.BMI;
}

mark.calcBMI = calcBMI;
john.calcBMI = calcBMI;


const getSummary = function() {
    if (this.mark.calcBMI() > this.john.calcBMI() ) {
        return `${this.mark.firstName}'s BMI (${this.mark.calcBMI()}) is higher than ${this.john.firstName}'s (${this.john.calcBMI()})`
    } else {
        return `${this.john.firstName}'s BMI (${this.john.calcBMI()}) is higher than ${this.mark.firstName}'s (${this.mark.calcBMI()})`
    }
}

const summary = {}
summary.mark = mark;
summary.john = john;
summary.getSummary = getSummary
console.log(mark.calcBMI(), john.calcBMI(), summary.getSummary())


for (let i=1; i<=10; i++){
    console.log(`😹  ${i}`)
}


const jonasArray = [
    'Jonas',
    'Schmedtman',
    2037 - 1991,
    'teacher',
    ['Michael', 'Peter', 'Steven'],
    true
];

const types = [];
for (let i = 0; i < jonasArray.length; i++) {
    console.log(jonasArray[i])
    types.push(typeof jonasArray[i])
};

const years = [1991, 2007, 2020, 1969];
const ages = [];

for (let i=0; i<years.length; i++){
    ages.push(2050 - years[i])
}
console.log(ages);

// continue and break
console.log('--- ONLY STRINGS ---')
for (let i = 0; i < jonasArray.length; i++) {
    if (typeof jonasArray[i] !== 'string') continue;
    console.log(jonasArray[i])
};

console.log('--- BREAK WITH NUMBER ---')
for (let i = 0; i < jonasArray.length; i++) {
    if (typeof jonasArray[i] === 'number') break;
    console.log(jonasArray[i])
};

console.log('--- BACKWARDS ---')
for (let i = jonasArray.length - 1; i >= 0; i--) {
    console.log(jonasArray[i])
}


const jonasArray = [
    'Jonas',
    'Schmedtman',
    2037 - 1991,
    'teacher',
    ['Michael', 'Peter', 'Steven'],
    true
];

for (let exercise = 1; exercise <= 4; exercise++) {
    console.log(`_______ Starting exercise ${exercise}` )

    for (let rep = 1; rep <= 5; rep++) {
        console.log(`Exercise ${exercise}: Lifting weight repetition ${rep} ☠` )
    }
}

// While Loop

let counter = 0;
while (counter <= 10) {
    console.log(`${counter}`)
    counter++
}

let dice = Math.trunc(Math.random() * 6) + 1;

while (dice !== 6) {
    console.log(`You rolled a 🎲 ${dice}`)
    dice = Math.trunc(Math.random() * 6) + 1;
    if (dice === 6) console.log('Loop is about to end ..')
}

const bills=[22, 295, 176, 440, 37, 105, 10, 1100, 86, 52];
const tips=[];
const totals=[];

const calcTip = function(bill) {
    return bill >= 50 && bill <=300 ? bill * 0.15: bill *0.2;
}

let i = 0;
console.log(`BILL \tTIP \tTOTAL`)
console.log(`=======================`)
while (i < bills.length) {
    tips.push(calcTip(bills[i]))
    totals.push(bills[i] + tips[i])
    console.log(`${bills[i]} \t${tips[i]} \t${totals[i]}`)
    i++
};

const calAverage = function(arr){
    let sum = 0
    for (i=0; i<arr.length; i++){
        sum += arr[i]
    }
    return sum/(arr.length)
}

const average = calAverage (totals)
console.log(`=======================`)
console.log(`Total Bill Average: ${average}`)
*/
