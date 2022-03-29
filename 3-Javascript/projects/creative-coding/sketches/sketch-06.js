const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');

const settings = {
  dimensions: [ 1080, 1080 ],
};

const sketch = () => {
  return ({ context, width, height, frame}) => {
    context.fillStyle = 'black';
    context.fillRect(0, 0, width, height);

    x = width  * 0.25;
    y = height * 0.25;
    w = width  * 0.5;
    h = height * 0.5;

    context.beginPath();
    context.save();
    context.rect(x, y, w, h);
    context.stroke();
    console.log(frame);

    for (i = 0; i < 36; i += 1) {
      context.translate((x + w * 0.5), (y + h * 0.5));
      context.rotate((Math.PI / 180) * 2.5);
      context.translate(-(x + w * 0.5), -(y + h * 0.5));
      context.rect(x, y, w, h);
      context.strokeStyle = 'white';
      context.stroke();
      context.restore();
    };
  };
};

canvasSketch(sketch, settings);