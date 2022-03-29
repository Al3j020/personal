const canvasSketch = require('canvas-sketch');
const math = require('canvas-sketch-util/math');
const random = require('canvas-sketch-util/random');

const settings = {
  dimensions: [ 1080, 1080 ],
  animate: true
};

const randomColor = () => {
  hex = Math.floor(Math.random()*16777215).toString(16);
  return "#" + hex;
}

const degToRad = (degrees) => {
  return degrees / 180 * Math.PI
}

const randomRange = (min, max) => {
  return Math.random() * (max - min) + min;
}

const sketch = () => {
  return ({ context, width, height }) => {
    context.fillStyle = 'black';
    context.fillRect(0, 0, width, height);
    context.fillStyle = 'black';

    const cx = width  * 0.5;
    const cy = height * 0.5;

    const w = width  * 0.01;
    const h = height * 0.25;
    let x, y;

    const num = 12;
    const radius = width * 0.5;
    const slice = math.degToRad(360 / num);

    for (let i = 0; i < num; i++) {
      const angle = slice * i

      x = cx + radius * Math.sin(angle);
      y = cy + radius * Math.cos(angle);

      context.save();
      context.translate(x, y);
      context.rotate(-angle);
      context.scale(random.range(0.1, 2), random.range(0.2, 0.5))
      context.lineWidth = random.range(0.5, 30);

      context.beginPath();
      context.fillStyle = randomColor()
      context.fillRect(-w * 0.5, random.range(0, -h * 0.5), w * random.range(0.1, 0.5), h * random.range(-2.5, 0));
      context.restore()

      context.save();
      context.translate(cx, cy);
      context.rotate(-angle);
      context.lineWidth = random.range(1, 10);

      context.beginPath();
      context.arc(0, 0, radius*random.range(0, 1), slice * (1, -10), slice * (1, 10))
      context.strokeStyle = randomColor()
      context.stroke();
      context.restore();

    }
  };
};

canvasSketch(sketch, settings);
