'use strict'
console.clear()
let row = [];
let matrix = []
const width = 11
const height = 11
const radius = 7

const circle = (radius, width, height) => {
    for (let dy = -width; dy <= height; dy += 0.25) {
    let str = ''
    for(let x = -width; x <= height; x += 0.25){
        let y = Math.pow((Math.pow(radius,2) - Math.pow(x,2)), 1/2)
        let roots = [y, -y]
        if (dy.toFixed() === roots[0].toFixed() || dy.toFixed() === roots[1].toFixed()){str += '$ '} else {str += '· '}
    }
        console.log(str);
    }
}

circle (radius, width, height)
// for (let dy = -height; dy <= height; dy += 0.5) {
//     let str = ''
//     for (let dx = -width; dx <= width; dx += 0.5){
//         let y = Math.pow((Math.pow(radius, 2) - Math.pow(dx, 2)), 1/2)
//         let roots = [y, -y]
//         if (dy.toFixed(1) === roots[0].toFixed(1)){str += '# '} else {str += '· '}
//         // console.log(`${dy}\t${dx}\t${roots[0].toFixed(1)}\t${roots[1].toFixed(1)}\t${radius<=roots[0]}\t${radius<=roots[1]}`);
//     }
//     console.log(str);
// }