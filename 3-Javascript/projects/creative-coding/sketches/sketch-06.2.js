const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');
const math = require('canvas-sketch-util/math');

const settings = {
  dimensions: [ 1080, 1080 ],
  animate: true,
};

const sketch = () => {
  return ({ context, width, height, frame }) => {
    context.fillStyle = 'black';
    context.fillRect(0, 0, width, height);
    const x = 0.5 * width;
    const y = 0.5 * height;
    const slice = 10;
    let radius = 0;
    context.scale(1, 1);
    const dash = [2,4,8,16,32];
    const pattern = []


    for (let i = 0; i <= width; i += 10) {
      radius += i;

      for (let j = 0; j < dash.length; j++) {
        pattern.push(random.pick(dash))
      };

      context.setLineDash(pattern);
      context.lineWidth = random.range(5, 50);
      context.beginPath();
      context.arc(x, y, radius, 0, 2*Math.PI);
      context.strokeStyle = 'white'
      context.stroke();
    };
  };
};

canvasSketch(sketch, settings);