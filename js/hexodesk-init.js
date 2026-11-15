(function(){
  let mode='dark';try{const stored=localStorage.getItem('hexodesk:blog-theme');if(stored==='light'||stored==='dark')mode=stored;}catch{}
  document.documentElement.dataset.blogTheme=mode;
  document.documentElement.style.colorScheme=mode;
})();
