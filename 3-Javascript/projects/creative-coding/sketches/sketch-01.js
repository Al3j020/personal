const canvasSketch = require('canvas-sketch');

const settings = {
  dimensions: [ 1080, 1080 ],
};

const sketch = () => {
  return ({ context, width, height }) => {
    context.fillStyle = 'black';
    context.fillRect(0, 0, width, height);
    context.lineWidth = width * 0.005;

    const w   = width   * 0.10;
    const h   = height  * 0.10;
    const gap = width   * 0.03;
    const ix  = width   * 0.17;
    const iy  = height  * 0.17;

    const off = width   * 0.02;

    for (let i = 0; i < 5; i++) {
        for (let j = 0; j < 5; j++) {

            let x = ix + (w + gap)*i;
            let y = iy + (w + gap)*j;

            context.beginPath();
            context.rect(x, y, w, h);
            context.strokeStyle = '#ffffff'
            context.stroke();

            if (Math.random() > 0.5) {
                context.beginPath();
                context.rect(x + off / 2, y + off / 2, w - off, h - off);
                context.strokeStyle = '#4287f5'
                context.stroke();
            }
        }
    }
  };
};

canvasSketch(sketch, settings);