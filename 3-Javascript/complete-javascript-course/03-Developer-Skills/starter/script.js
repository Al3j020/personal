// Remember, we're gonna use strict mode in all scripts now!
'use strict';
/*
const calcTempAmplitude = function (temps) {
    let max = temps[0];
    let min = temps[0];

    for (let i = 0; i < temps.length; i++) {
        const curTemp = temps[i];
        if (typeof curTemp !== 'number') continue;
        if (curTemp > max) max = curTemp;
        if (curTemp < min) min = curTemp;
    }
    console.log(max, min);
    return max - min;
};

const amplitude = calcTempAmplitude([3, 7, 4, 1, 'error', 8, 9, 25]);
console.log(amplitude);
*/
/*
const measureKelvin = function () {
    const measurement = {
        type: 'temp',
        unit: 'celsius',
        value: 20, //Number(prompt('Degrees celsius: ')),
    };
    console.table(measurement);
    console.log(typeof measurement.value);
    const kelvin = measurement.value + 273;
    return kelvin;
};

console.log(measureKelvin());
*/

/*
const temps = [12, 5, -5, 0, 4];
console.log('printForecasts...', temps.length);

const printForecasts = function (arr) {
    let str = '';
    for (let i = 0; i < arr.length; i++) {
        str += `... ${arr[i]}ºC in ${i + 1} days `;
    }
    console.log(str);
};
printForecasts(temps);
*/
