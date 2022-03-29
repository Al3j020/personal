console.log("script.js");
/*
console.log(40+8+23-10);
console.log('Jonas');
console.log(23);

// Variable name conventions
let firstName = 'Matilda';
let $function = 27;
let pi = 3.1416;
let person = "Jonas"

let myFirstJob = "programmer";
let myCurrentJob = "teacher";

let job1 = "programmer";
let job2 = "teaches";

// ilegal variables names //
// let 3first
// let jonas&matilda
// let new = 27
// let function = 'Jonas'
// let name = 'Matilda'

console.log(firstName);
console.log(firstName);
console.log(firstName);
console.log($function);
console.log(pi);
console.log(person);
console.log(myFirstJob);


// The 7 primitive data types
// 1. Number: Floating point numbers ➡️ Used for decimal and integers
// 2. String: Sequence of characters ➡️ Used for text
// 3. Boolean: Logical type that can only be true or false ➡️ Used for taking decisions
// 4. Undefined : Value taken by a varibale that is not yet defined (''empty value) ➡️ let children;
// 5. Null; also means 'empty value'
// 6. Symbol(ES2015): Value that is unique and cannot be changed ➡️ [Not useful for now]
// 7. BigInt(ES2020): Larger integers than the number type can hold

let javascriptIsFun = true;
console.log(javascriptIsFun);
console.log(typeof javascriptIsFun);
console.log(typeof false);
console.log(typeof 23);
console.log(typeof "Jonas");

javascriptIsFun = "YES!"
console.log(javascriptIsFun);
console.log(typeof javascriptIsFun);

let  year;
console.log(year);
console.log(typeof year);

year = 1991;
console.log(year);
console.log(typeof year);

//🐞 on Javascript null is not an object
console.log(typeof null)

// let - const - var
let age = 30;
age = 31; //reassigning

const birthYear = 1991;
// birthYear = 1990; --> TypeError: Assignment to constant variable.
//const job; --> SyntaxError: Missing initializer in const declaration

var job = "programmer";
job = "teacher";

lastName = "Smith"; //--> don't write variables down in this way
console.log(lastName);

//Basic Operators

const now = 2037
const ageJonas = now - 1991;
const ageSarah = now - 2018;
console.log("ages: ",ageJonas, ageSarah);
console.log(ageJonas * 2, ageJonas / 10, 2** 3);
// 2 ** 3 means 2 to the power of 3 = 2 * 2 * 2
const firstName = "John";
const lastName = "Doe"
console.log(firstName + " " + lastName)

//Assignment operators
let x = 10 + 5;
x += 10; // x = x + 10 = 25
x -= 10; // x = x - 10
x *= 4; // x = x * 4
x /= 9; // x = x / 9
x++;
x--;
console.log(x);

//Comparison operators

console.log(ageJonas > ageSarah); // >,<, >=,<=
const isFullAge = ageSarah >= 18;
console.log(now - 1991 > now - 2018);

let y, z;
y = z = 25 - 10 - 5; // y = z = 10; y = 10;
console.log(y, z);

const averageAge = (ageJonas + ageSarah) / 2;
console.log(ageJonas, ageSarah, averageAge);

const massMark      = 78;
const heightMark    = 1.69;
const massJohn      = 92;
const heightJohn    = 1.95;
let markHigherBMI;

BMIMark = massMark / (heightMark ** 2);
BMIJohn = massJohn / (heightJohn * heightJohn);
markHigherBMI = BMIMark > BMIJohn;
console.log(BMIMark, BMIJohn, markHigherBMI);

const firstName = "Jonas";
const job = "teacher";
const birthYear = 1991;
const year = 2037;
const jonas = "I'm " + firstName + ", a " + (year - birthYear) + " years old " + job + "!";
console.log(jonas);

const jonasNew = `I'm ${firstName}, a ${year - birthYear} year old ${job}!`;

console.log(jonasNew);
console.log(`Just a regular string... `);

// back tips ``
console.log(`I can't connect to the server`);
console.log(`String with
multiple
lines`);

// simple quotes ''
console.log('I can\'t connect to the server');
console.log(`I can't connect to the server`);

// double quotes ""
console.log("Error: \"Introduce a valid email\"");
console.log(`Error: "Introduce a valid email"`);

//class 05.10.2021
//If/Else Staments
const age = 15;
const isOldEnough = age >= 18;

if (isOldEnough) {
    console.log("Sarah can start driving license 😃");
} else {
    const yearsLeft = 18 - age;
    console.log(`Sarah is too young. Wait another ${yearsLeft} years 😉`);
}

const birthYear = 2014;
let century;

if (birthYear <= 2000){
    century = 20
} else {
    century = 21
}
console.log(century)

//Challenge

const massMark      = 78;
const heightMark    = 1.69;
const massJohn      = 92;
const heightJohn    = 1.95;
let markHigherBMI;

BMIMark = massMark / (heightMark ** 2);
BMIJohn = massJohn / (heightJohn * heightJohn);

if (BMIMark > BMIJohn) {
    console.log(`Mark's BMI (${BMIMark}) is higher than John's (${BMIJohn})`);
} else {
    console.log(`John's BMI  (${BMIJohn}) is higher than Mark's (${BMIMark})`);
}

//Type conversion and coercion

//Type conversion
const inputYear = '1991';
console.log(Number(inputYear), inputYear) //--> 1991, "1991"
console.log(Number(inputYear) + 18);
console.log(Number("Alejo"));
console.log(typeof NaN);

console.log(typeof String(23), String(23), typeof 23, 23);

//Type coersion
console.log('I am ' + 31 + ' years old');
console.log('I am ' + '31' + ' years old');
console.log('23' + '10' + 3);   // [23103] string concatenated
console.log('23' - '10' - 3);   // [10] number operation
console.log('23' * '10' * 3);   // [690] number operation
console.log('23' / '10' / 3);   // [0.76] number operation
console.log('23' > 10);         // [true] boolean operation

let n = '1' + 1;
n = n - 1;
console.log(n);


//Truthy and Falsy Values
// 5 falsy values: 0, '', undefined, null, NaN

console.log(Boolean(0));
console.log(Boolean(''));
console.log(Boolean(undefined));
console.log(Boolean(null));
console.log(Boolean(NaN));

console.log('***************');

console.log(Boolean(1));
console.log(Boolean({}));
console.log(Boolean('Jonas'));

const money = 0;

if (money) {
    console.log("Don't spent it all 😘");
} else {
    console.log("You should get a job! 🤷‍♂️");
}

let height;
//let height = 0;
console.log(typeof height)

if (height) {
    console.log('YAY! Height is defined 😊');
} else {
    console.log('Height is UNDEFINED 😒')
}

//EQUALITY OPERATORS: == VS ===
const age = '18';

if (age == 18) console.log('You just became an adult XD (loose)');
if (age === 18) console.log('You just became an adult XD (strict)');

// [=] assignment, [==] type coersion, [===] strict equality *highly recommend to use
console.log('19' == 19);  // --> TRUE
console.log('19' === 19); // --> FALSE

const favourite = prompt("What's your favorite number?");
console.log(favourite);
console.log(typeof favourite);

if (favourite === 23) {
    console.log(`Cool! 23 is amazing number`)
} else if (favourite === 7) {
    console.log(`7 is also a cool number`)
} else if (favourite === 7) {
    console.log(`9 is also a cool number`)
} else {
    console.log(`Number is not 23 OR 7 OR 9`)
}

// [!==] strict inequality
if (favourite !== 23) console.log('Why not 23?')


//BASIC BOOLEAN LOGIC: THE AND, OR & NOT OPERATORS

//LOGICAL OPERATORS

const hasDriversLicense = true;
const hasGoodVision = true;

console.log(hasDriversLicense && hasGoodVision); // [AND]
console.log(hasDriversLicense || hasGoodVision); // [OR]
console.log(!hasDriversLicense); //[NOT]

const shouldDrive = hasDriversLicense && hasGoodVision;

if (shouldDrive) {
    console.log("Sarah is able to drive!");
} else {
    console.log("Someone else should drive...");
}

const isTired = true;
console.log(hasDriversLicense || hasGoodVision || isTired);

if (hasDriversLicense && hasGoodVision && !isTired) {
    console.log("Sarah is able to drive!");
} else {
    console.log("Someone else should drive...");
}

//CODING CHALLENGE

let scoreDolphins = 97 + 112 + 101;
let scoreKoalas = 109 + 95 + 123;
let minimumScore = 100;

let dolphinsScoreAvr = scoreDolphins/3;
let koalasScoreAvr = scoreKoalas/3;
console.log(dolphinsScoreAvr, koalasScoreAvr);

if (dolphinsScoreAvr > koalasScoreAvr && dolphinsScoreAvr >= minimumScore) {
    console.log('Dolphins win the trophy 🏆');

} else if (dolphinsScoreAvr < koalasScoreAvr &&  koalasScoreAvr >= minimumScore) {
    console.log('Koalas win the trophy 🏆');

} else if (dolphinsScoreAvr === koalasScoreAvr && koalasScoreAvr >= minimumScore && dolphinsScoreAvr >= minimumScore) {
    console.log('Both win the trophy');

} else {
    console.log('No one wins the trophy');
}


//THE SWITCH STATEMENT

const day = prompt("Today is:")

console.log('switch statement');
switch(day){
    case 'monday': // day === 'monday'
        console.log('Plan course structure');
        console.log('Go to coding meetup');
        break;
    case 'tuesday':
        console.log('Prepare theory videos');
        break;
    case 'wednesday':
    case 'thursday':
        console.log('Write code examples');
        break;
    case 'friday':
        console.log('Record videos');
        break;
    case 'saturday':
    case 'sunday':
        console.log('Enjoy the weekend :D');
        break;
    default:
        console.log('Not a valid day!')
}

console.log('if statement');
if (day === 'monday'){
    console.log('Plan course structure');
    console.log('Go to coding meetup');

} else if (day === 'tuesday'){
    console.log('Prepare theory videos');

} else if (day === 'wednesday' || day === 'thursday') {
    console.log('Write code examples');

} else if (day === 'friday') {
    console.log('Record videos');

} else if (day === 'saturday' || day === 'sunday') {
    console.log('Enjoy the weekend :D');

} else {
    console.log('Not a valid day!');
}


//STATEMENTS AND EXPRESIONS

// Expresions
3 + 4
1991
true && false || !false

// Statements
if (23 > 10) {
    const str = '23 is bigger';
}

console.log(`I'm ${2021 - 1990} years old`); //Expresion

//CONDITIONAL (TERNARY) OPERATOR

const age = 17;
age >= 18 ? console.log('I like to drink wine 🍷'): console.log('I like to drink water 🧊');

const drink = age >= 18 ? 'wine 🍷' : 'water 🧊';
console.log(drink);

console.log(`I like to drink ${age >= 18 ? 'wine 🍷' : 'water 🧊'}`);

const bill = 430;
let tip;
let tipPercent;
// const tip = bill >= 50 && bill <= 300 ? bill*0.15 : bill*0.2;
bill >= 50 && bill <= 300 ? tipPercent = 0.15 : tipPercent = 0.2;
console.log(tipPercent);
tip = bill*tipPercent;
console.log(`The bill was $${bill}, the tip was $${tip}, and the total value was $${bill + tip}`)

// The bill was $275, the tip was $41.25, and the total value was $316.25
// The bill was $40, the tip was $8, and the total value was $48
// The bill was $430, the tip was $86, and the total value was $516
*/
console.log("<------ node js ------->");
const favourite = prompt("What's your favorite number?");
console.log(favourite);
console.log(typeof favourite);