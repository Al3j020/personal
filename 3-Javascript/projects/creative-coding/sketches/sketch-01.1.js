const canvasSketch = require('canvas-sketch');
const random = require('canvas-sketch-util/random');

const settings = {
  dimensions: [ 1080, 1080 ],
};

const sketch = () => {
  return ({ context, width, height }) => {
    context.fillStyle = 'white';
    context.fillRect(0, 0, width, height);
    context.lineWidth = width * 0.005;

    const w   = width   * 0.10;
    const h   = height  * 0.10;
    const gap = width   * 0.03;
    const ix  = width   * 0.17;
    const iy  = height  * 0.17;

    const off = width   * 0.02;
    console.log(width, height, w, h, gap, ix, iy);
    for (let i = 0; i < 5; i++) {
        for (let j = 0; j < 5; j++) {

            let x = ix + (w + gap)*i;
            let y = iy + (w + gap)*j;

            // context.beginPath();
            // context.rect(x, y, w, h);
            // context.strokeStyle = '#ffffff'
            // context.stroke();
            console.log('[x, y] : ', x, y)
            context.save();
            context.translate(x, y);
            context.lineWidth = 2;

            context.beginPath();
            context.fillStyle = randomColor();
            context.arc(0, 0, 25, 0, Math.PI * 2);
            context.fill();
            // context.stroke();

            context.restore();

            // if (Math.random() > 0.5) {
            //     context.beginPath();
            //     context.rect(x + off / 2, y + off / 2, w - off, h - off);
            //     context.strokeStyle = '#4287f5'
            //     context.stroke();
            // }
        }
    }
  };
};

canvasSketch(sketch, settings);

const randomColor = () => {
  hex = Math.floor(Math.random()*16777215).toString(16);
  return "#" + hex;
}