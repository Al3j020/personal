const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');
const math = require('canvas-sketch-util/math');
const TweakPane = require('tweakpane');

const settings = {
  dimensions: [ 1200, 1200 ],
  // animate:true,
};

const params = {
  cols: 2,
  rows: 2,
  numSides: 12,
  radius: 500,
  freq: 0.001,
  color: Math.PI,
  amp: 0,
  frame: 10,
  h: 0,
  s: 100,
  l: 50,
};

const hlsColor = (noise) => {
  const h = math.mapRange(Math.tan(params.h * Math.PI/180) * noise, -1, 1, 0, 360);
  const s = math.mapRange(params.s * noise, -1, 1, 0, 100);
  return `hsl(${h}, ${s}%, ${params.l}%)`;
};

const sketch = () => {
  return ({ context, width, height, frame }) => {
    context.fillStyle = 'white';
    context.fillRect(0, 0, width, height);

    const coordinates = [];
    const radius = params.radius;
    const cx = width  * 0.5;
    const cy = height * 0.5
    const step = (2 * Math.PI) / params.numSides
    // let i = -1;

    // context.save()
    // context.beginPath();
    // context.strokeStyle = 'purple'
    // context.moveTo(cx + radius, cy);
    // for (let theta = 0; theta <= 2 * Math.PI; theta += step ) {
    //   let x = cx + radius * Math.cos(theta);
    //   let y = cy + radius * Math.sin(theta);
    //   coordinates.push([x,y]);
    //   context.lineTo(x, y);
    //   context.stroke();
    // }
    // context.restore();

    for (let theta = 0; theta <= 2 * Math.PI; theta += step ) {
      let x = cx + radius * Math.cos(theta);
      let y = cy + radius * Math.sin(theta);
      coordinates.push([x,y]);
    }
    const r = random.pick([1,2,3,4,5,6,7,8,9,10]);
    console.log(r);
    for (let i = 0; i < params.numSides; i++){
      let x0 = coordinates[i + 0][0];
      let y0 = coordinates[i + 0][1];
      let x1 = coordinates[i + 1][0];
      let y1 = coordinates[i + 1][1];
      context.save();
      context.beginPath();
      context.lineCap = 'round';
      context.lineWidth = 5;
      if(i % r) {context.strokeStyle = 'white'} else {context.strokeStyle = 'black'};
      context.moveTo(x0, y0);
      context.lineTo(x1, y1);
      context.stroke();
      context.restore()
    };

    // for (let theta = 0; theta <= 2 * Math.PI; theta += step ) {
    //   let x = cx + radius * Math.cos(theta);
    //   let y = cy + radius * Math.sin(theta);
    //   context.save();
    //   context.strokeStyle = 'black'
    //   context.beginPath();
    //   context.arc(x, y, random.range(1, radius * 0.2), 0, 2 * Math.PI);
    //   context.stroke();
    //   context.restore();
    // }
  };
};

canvasSketch(sketch, settings);

class Polygon {
  constructor(cx, cy, radius, n, angle) {
    this.cx = cx;
    this.cy = cy;
    this.radius = radius;
    this.n = n;
    this.angle = angle;
  };

  draw(context) {
    const step = (2 * Math.PI) / this.n
    for (let theta = 0; theta <= 2 * Math.PI; theta += step ) {
      let x = this.cx + this.radius * Math.cos(theta);
      let y = this.cy + this.radius * Math.sin(theta);
      context.save();
      context.beginPath();
      context.arc(x, y, 10, 0, 2 * Math.PI);
      context.fill();
      // context.lineTo(x, y);
      context.stroke();
      context.restore();
    }
  };
};

const createPane = () => {
  const pane = new TweakPane.Pane();
  let folder;

  folder = pane.addFolder({ title: 'Grid' });
  folder.addInput(params, 'cols', { min: 1, max: 100, step: 2 });
  folder.addInput(params, 'rows', { min: 1, max: 100, step: 2 });
  folder.addInput(params, 'numSides', { min: 2, max: 360, step: 1 });
  folder.addInput(params, 'radius', { min: 50, max: 500, step: 0.1 });

  folder = pane.addFolder({ title: 'Noise' });
  folder.addInput(params, 'freq', { min: -0.01, max: 0.01 });
  folder.addInput(params, 'amp', { min: 0, max: 2*Math.PI });
  folder.addInput(params, 'frame', { min: 1, max: 20, step: 0.5 });

  folder = pane.addFolder({ title: 'Color (hls)' });
  folder.addInput(params, 'h', { min: 0, max: 360 });
  folder.addInput(params, 's', { min: 0, max: 100 });
  folder.addInput(params, 'l', { min: 0, max: 100 });
};

createPane();