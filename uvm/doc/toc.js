(function () {
  // build the table of contents from the headings
  var toc = document.getElementById('toc');
  var heads = Array.prototype.slice.call(document.querySelectorAll('#doc h1[id], #doc h2[id]'));
  var links = heads.map(function (h) {
    var a = document.createElement('a');
    a.href = '#' + h.id;
    a.className = h.tagName === 'H1' ? 'l1' : 'l2';
    var n = h.querySelector('.n');
    var num = document.createElement('span'); num.className = 'tn'; num.textContent = n ? n.textContent : '';
    a.appendChild(num);
    a.appendChild(document.createTextNode(h.textContent.slice(n ? n.textContent.length : 0)));
    toc.appendChild(a);
    a.addEventListener('click', function () { toc.classList.remove('open'); });
    return a;
  });
  document.getElementById('tocbtn').addEventListener('click', function () { toc.classList.toggle('open'); });
  // highlight the section being read
  function spy() {
    var y = window.scrollY + 90, cur = 0;
    for (var i = 0; i < heads.length; i++) if (heads[i].offsetTop <= y) cur = i;
    links.forEach(function (a, i) { a.classList.toggle('active', i === cur); });
    var act = links[cur];
    if (act && (act.offsetTop < toc.scrollTop || act.offsetTop > toc.scrollTop + toc.clientHeight - 40))
      toc.scrollTop = act.offsetTop - 80;
  }
  window.addEventListener('scroll', spy, {passive: true});
  spy();
})();
