const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');
const math = require('canvas-sketch-util/math');
const TweakPane = require('tweakpane');

const settings = {
  dimensions: [ 1080, 1080 ],
  animate:true,
};

const params = {
  radius1: 300,
  radius2: 150,
  ratio: 25,
  freq: 0.5,
};

const sketch = () => {
  return ({ context, width, height, frame }) => {
    context.fillStyle = 'white';
    context.fillRect(0, 0, width, height);
    const cx = width  * 0.5;
    const cy = height * 0.5;

    const radius1 = params.radius1;
    const radius2 = params.radius2;
    const ratio = params.ratio;

    context.beginPath();
    context.lineWidth = 1.5;
    context.moveTo(cx + radius1 + radius2, cy);
    for (theta = 0; theta <= 2 * Math.PI; theta += 0.001) {
      x = cx + radius1 * Math.cos(theta)  + radius2 * Math.cos(theta * ratio);
      y = cy + radius1 * Math.sin(theta)  + radius2 * Math.sin(theta * ratio);

      context.lineTo(x.toFixed(0), y.toFixed(0));
    }

    const radians = frame * Math.PI/180 * params.freq;
    const hue = 180;
    context.strokeStyle = `hsl(${hue * (1 + Math.cos(radians))}, 100%, 50%)`
    console.log(hue * Math.sin(radians));
    context.stroke();
  };
};

canvasSketch(sketch, settings);

class Polygon {
  constructor(cx, cy, radius, sides, angle) {
    this.cx = cx;
    this.cy = cy;
    this.radius = radius;
    this.sides = sides;
    this.angle = angle;
  };

  rotate(context) {
    context.rotate(this.angle);
  };


  draw(context) {
    context.save();
    context.beginPath();

    for (let i = 0; i <= this.sides; i ++) {
      this.angle += Math.PI * 2/this.sides
      let x = this.radius*Math.cos(this.angle);
      let y = this.radius*Math.sin(this.angle);
      context.lineTo(x.toFixed(2), y.toFixed(2));
      context.stroke();
    };
    context.restore();
  };
};

const createPane = () => {
  const pane = new TweakPane.Pane();
  let folder;

  folder = pane.addFolder({ title: 'Params' });
  folder.addInput(params, 'ratio', { min: 1, max: 100, step: 1 });
  folder.addInput(params, 'radius1', { min: 100, max: 500, step: 10 });
  folder.addInput(params, 'radius2', { min: 50, max: 250, step: 10 });
  folder.addInput(params, 'freq', { min: 0, max: 2 , step: 0.1});
};

createPane();