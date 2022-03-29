const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');
const math = require('canvas-sketch-util/math');
const TweakPane = require('tweakpane');

const settings = {
  dimensions: [ 1200, 1200 ],
  animate:true,
};

const params = {
  cols: 2,
  rows: 2,
  numSides:4,
  prop: 0.5,
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
    context.fillStyle = 'black';
    context.fillRect(0, 0, width, height);
    const numSides = params.numSides;
    const cols = params.cols;
    const rows = params.rows;
    const numCells = cols * rows;

    const gridw = width  * 0.8;
    const gridh = height * 0.8;
    const cellw = gridw / cols;
    const cellh = gridh / rows;
    const margx = (width - gridw) * 0.5;
    const margy = (height - gridh) * 0.5;

    context.lineWidth = 2.5
    for (let i = 0; i < numCells; i++) {
      const col = i % cols;
      const row = Math.floor(i / cols);

      const x = (margx + cellw * 0.5) + col * cellw;
      const y = (margy + cellh * 0.5) + row * cellh;
      console.log(col, row, x,y);
      const noise = random.noise3D(x, y, frame * params.frame, params.freq);
      const angle =  noise * params.amp;
      const radius = cellw * params.prop;

      context.strokeStyle =  hlsColor(noise);
      const polygon = new Polygon(x, y, radius, numSides, angle);
      polygon.draw(context);

    };
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
    context.save();
    context.beginPath();
    for (let i = 0; i <= this.n; i++) {
      this.angle += Math.PI * 2/this.n
      let x = this.cx + this.radius*Math.cos(this.angle);
      let y = this.cy + this.radius*Math.sin(this.angle);
      context.lineTo(x.toFixed(2), y.toFixed(2));
      context.stroke();
    };

    context.rotate(this.angle);
    context.restore();
  };
};

const createPane = () => {
  const pane = new TweakPane.Pane();
  let folder;

  folder = pane.addFolder({ title: 'Grid' });
  folder.addInput(params, 'cols', { min: 1, max: 100, step: 2 });
  folder.addInput(params, 'rows', { min: 1, max: 100, step: 2 });
  folder.addInput(params, 'numSides', { min: 2, max: 20, step: 1 });
  folder.addInput(params, 'prop', { min: 0.1, max: 5, step: 0.1 });

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