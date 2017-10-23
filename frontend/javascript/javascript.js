reloadImages();
window.onresize = function(event) {
  reloadImages();
};
function reloadImages(){
  var istream_1 = document.getElementById("istream_1");
  var istream_1_width = document.getElementById("istream_1_wrap").clientWidth;
  var istream_1_height = document.getElementById("istream_1_wrap").clientHeight;
  istream_1.width=istream_1_width;
  istream_1.height=istream_1_height;
  var ctx = istream_1.getContext("2d");
  var img = new Image();
  img.src='../../c++/old.jpg';
  img.onload = function() {
    drawImageProp(ctx, img, 0, 0, istream_1_width, istream_1_height, 0.1, 0.5);
  };
  var istream_2 = document.getElementById("istream_2");
  var istream_2_width = document.getElementById("istream_2_wrap").clientWidth;
  var istream_2_height = document.getElementById("istream_2_wrap").clientHeight;
  istream_2.width=istream_2_width;
  istream_2.height=istream_2_height;
  var ctx2 = istream_2.getContext("2d");
  var img2 = new Image();
  img2.src='../../c++/old.jpg';
  img2.onload = function() {
    drawImageProp(ctx2, img2, 0, 0, istream_1_width, istream_1_height, 0.1, 0.5);
  };

}
function drawImageProp(ctx, img, x, y, w, h, offsetX, offsetY) {

  if (arguments.length === 2) {
      x = y = 0;
      w = ctx.canvas.width;
      h = ctx.canvas.height;
  }

  /// default offset is center
  offsetX = typeof offsetX === 'number' ? offsetX : 0.5;
  offsetY = typeof offsetY === 'number' ? offsetY : 0.5;

  /// keep bounds [0.0, 1.0]
  if (offsetX < 0) offsetX = 0;
  if (offsetY < 0) offsetY = 0;
  if (offsetX > 1) offsetX = 1;
  if (offsetY > 1) offsetY = 1;

  var iw = img.width,
      ih = img.height,
      r = Math.min(w / iw, h / ih),
      nw = iw * r,   /// new prop. width
      nh = ih * r,   /// new prop. height
      cx, cy, cw, ch, ar = 1;

  /// decide which gap to fill
  if (nw < w) ar = w / nw;
  if (nh < h) ar = h / nh;
  nw *= ar;
  nh *= ar;

  /// calc source rectangle
  cw = iw / (nw / w);
  ch = ih / (nh / h);

  cx = (iw - cw) * offsetX;
  cy = (ih - ch) * offsetY;

  /// make sure source rectangle is valid
  if (cx < 0) cx = 0;
  if (cy < 0) cy = 0;
  if (cw > iw) cw = iw;
  if (ch > ih) ch = ih;

  /// fill image in dest. rectangle
  ctx.drawImage(img, cx, cy, cw, ch,  x, y, w, h);
}
