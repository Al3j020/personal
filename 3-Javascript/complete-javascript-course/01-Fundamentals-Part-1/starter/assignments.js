console.log("--------------------------------------")
console.log("assignments.js")

const country   = "Canada"//"Colombia";
const continent = "América";
const isIsland  = false;

let population  = 45;
let colombiaPopulation  = 45;
let language = "english";//"spanish";

console.log("Country: " + country);
console.log("Continent: " + continent);
console.log("colombiaPopulation : " + colombiaPopulation + " millions");
console.log("is an Island: " + isIsland);

console.log("--> LECTURE: Basic Operators")

console.log(colombiaPopulation/2);
colombiaPopulation += 1;
console.log(colombiaPopulation);

let finlandPopulation = 6;
console.log(colombiaPopulation > finlandPopulation)

let averagePopulation = 33;
console.log(colombiaPopulation < averagePopulation)
// let description = "";
// description = country + " is in "+ continent + ", and its " + colombiaPopulation + " million people speak " + language;
// console.log(description);

let description = "";
description = `${country} is in ${continent}, and its ${colombiaPopulation} million people speak ${language}`;
console.log(description);

console.log("--> LECTURE: Taking Decisions: if / else Statements")

if (colombiaPopulation > 33) {
    console.log(`${country}'s population is above average`)
} else {
    console.log(`${country}'s population is ${colombiaPopulation} million below average`)
}

console.log("--> LECTURE: Type Conversion and Coercion")
console.log('9' - '5');                 //4
console.log('19' - '13' + '17');        // 617
console.log('19' - '13' + 17);          //23
console.log('123' < 57);                // false
console.log(5 + 6 + '4' + 9 - 4 - 2);   // 1149

// console.log("--> LECTURE: Equality Operators: == vs. ===")
// let numNeighbours = prompt('How many neighbour countries does your country have?');

// if (numNeighbours == 1 ){
//     console.log('Only 1 border!');
// } else if (numNeighbours > 1){
//     console.log('More than 1 border');
// } else if (numNeighbours == 0){
//     console.log('No borders')
// }

// if (numNeighbours === 1 ){
//     console.log('Only 1 border!');
// } else if (numNeighbours > 1){
//     console.log('More than 1 border');
// } else if (numNeighbours === 0){
//     console.log('No borders')
// }

console.log("--> LECTURE: Logical Operators")

if (language === 'english' && population <= 50 && !isIsland ){
    console.log(`You should live in : ${country} 😊`);
} else {
    console.log(`${country} does not meet your criteria 😒`);
}