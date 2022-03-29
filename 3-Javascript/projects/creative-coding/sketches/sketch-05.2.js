const canvasSketch = require('canvas-sketch');
const load = require('load-asset');
const random = require('canvas-sketch-util/random');
const math = require('canvas-sketch-util/math');
const TweakPane = require('tweakpane');

const settings = {
  dimensions: [ 1080, 1080 ],
  animate: true,
};

let manager, image;

let fontSize = 1200;
let fontFamily = 'C64ProMono-10';

const params = {
  cells: 10,
  color: 'white',
  fontSize: 15,
  randomSize: true,
  freq: 0,
};

const typeCanvas = document.createElement('canvas');
const typeContext = typeCanvas.getContext('2d');

const sketch = ({ context, width, height }) => {

  return ({ context, width, height, frame }) => {

    const cell = params.cells;
    const cols = Math.floor(width  / cell);
    const rows = Math.floor(height / cell);
    const numCells = cols * rows;
    typeCanvas.width  = cols;
    typeCanvas.height = rows;
    console.log(cell, cols, rows);
    typeContext.fillStyle = 'black';
    typeContext.fillRect(0, 0, cols, rows);

    typeContext.drawImage(image, 0, 0, cols, rows);

    const typeData = typeContext.getImageData(0, 0, cols, rows).data;

    // fontSize = cols;

    typeContext.fillStyle = 'white';
    typeContext.font = `${fontSize}px ${fontFamily}`;
    typeContext.textBaseLine = 'top';

    context.fillStyle = 'black';
    context.fillRect(0, 0, width, height);

    context.textBaseLine = 'middle';
    context.textAlign = 'center';

    for (let i = 0; i < numCells; i++) {
      const col = i % cols;
      const row = Math.floor(i / cols);

      const x = col * cell;
      const y = row * cell;

      const r = typeData[i * 4 + 0];
      const g = typeData[i * 4 + 1];
      const b = typeData[i * 4 + 2];
      const a = typeData[i * 4 + 3];

      const glyph = getGlyph(r);

      const n = random.noise2D((x + frame), y, params.freq);
      // const n = random.noise3D(x, y, frame * 10, 0.01);
      const fontSize = math.mapRange(n, -1, 1, 1, params.fontSize)
      context.font = `${fontSize}px ${fontFamily}`;
      context.fillStyle = params.color;
      if( glyph === 'X')  context.fillStyle = 'purple';
      context.save();
      context.translate(x, y);
      context.translate(cell * 0.5, cell * 0.5);
      context.fillText(glyph, 0, 0);
      context.restore();
    };
  };
};

const getGlyph = (v) => {
  const glyphs = ':;/▁▂▃░⋰_◜◠+*`=?!¬▩▪▫▬▭▮▯▰▱◖◗◜◝◞◟◠◡◸◹◺◿'.split('');
  // if (v < 50)  return '.';
  // if (v < 100) return '×';
  // if (v < 150) return '✕';
  // if (v < 200) return '✖';

  if (v < 50)  return '';
  if (v < 100) return ',';
  if (v < 150) return 'x';
  if (v < 200) return 'X';

  return '';
};

const createPane = () => {
  const pane = new TweakPane.Pane();
  let folder;

  folder = pane.addFolder({ title: 'Grid' });
  folder.addInput(params, 'color', { options: { red: 'red', green : 'green', blue: 'blue', orange : 'orange', white: 'white' }});
  folder.addInput(params, 'cells', { min: 5, max: 20, step: 1 });
  folder.addInput(params, 'freq', { min: -0.01, max: 0.01, step: 0.001 });
  folder = pane.addFolder({ title: 'Font' });
  folder.addInput(params, 'fontSize', { min: 5, max: 50, step: 1 });
  folder.addInput(params, 'randomSize');
};

createPane();

const loadImage = (url) => {
  return new Promise((resolve, reject) => {
    const img = new Image();
    img.onload = () => resolve(img);
    img.onerror = () => reject();
    img.src = url;
  });
};

const start = async () => {
  const url = './test-13.jpg';
  image = await loadImage(url);
  manager = await canvasSketch(sketch, settings);
};

start();