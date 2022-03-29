const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');
const math = require('canvas-sketch-util/math');

const settings = {
  dimensions: [ 1080, 1080 ],
  // animate: true,
};

const sketch = () => {
  return ({ context, width, height, frame }) => {
    context.fillStyle = 'white';
    context.fillRect(0, 0, width, height);

    const ix = width  * 0;
    const iy = height * 0;

    const fx = width  * 1.75;
    const fy = height * 0;
    const angle = 90;
    const n = 5;

    let pattern = [];
    context.lineWidth = random.range(0.5, 10);
    // context.scale(random.range(0.5, 1.5), random.range(0.5, 1.5));
    context.scale(1.25, 1.25);
    let nn
    let tmp = 1;

    for (let i = 0; i <= angle/n; i += 1) {
      tmp *= -1;
      nn = random.noise2D(Math.sin((Math.PI / 180) * tmp), Math.cos((Math.PI / 180) * tmp), 0.01);
      pattern.push(Math.pow(2,nn) + tmp);
    };

    for (let i = 0; i <= angle/n; i += 1) {
      context.beginPath();
      context.setLineDash(pattern);
      context.moveTo(ix, iy);
      context.lineTo(fx, fy);
      context.stroke();
      context.rotate((Math.PI / 180) * n);
    };
  };
};

canvasSketch(sketch, settings);