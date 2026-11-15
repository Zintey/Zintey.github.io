(function(){
  let serial=0;
  function setOpen(section,open){const content=section.querySelector(':scope > .desk-section-content'),button=section.querySelector(':scope > :is(h1,h2,h3,h4,h5,h6) > .desk-section-toggle');if(!content||!button)return;content.hidden=!open;button.setAttribute('aria-expanded',String(open));section.classList.toggle('is-collapsed',!open);button.title=open?'收起章节':'展开章节';}
  function install(){
    const entry=document.querySelector('#post .article-entry');if(!entry)return;
    const headings=[...entry.querySelectorAll('h1,h2,h3,h4,h5,h6')].filter(h=>!h.closest('pre,code,summary'));
    const containers=new Set(headings.map(h=>h.parentElement));
    for(const root of containers){
      if(root.dataset.deskSectionRoot)continue;root.dataset.deskSectionRoot='true';const stack=[];
      for(const node of [...root.childNodes]){
        const level=node.nodeType===1&&/^H[1-6]$/.test(node.tagName)?Number(node.tagName[1]):0;
        if(level){
          while(stack.length&&stack.at(-1).level>=level)stack.pop();
          const section=document.createElement('section'),content=document.createElement('div'),button=document.createElement('button');
          section.className='desk-section';content.className='desk-section-content';content.id='desk-section-body-'+(++serial);
          button.type='button';button.className='desk-section-toggle';button.setAttribute('aria-label','折叠或展开：'+node.textContent.trim());button.setAttribute('aria-controls',content.id);button.innerHTML='<svg viewBox="0 0 16 16" aria-hidden="true"><path d="m5 3 5 5-5 5"/></svg>';
          node.prepend(button);section.append(node,content);(stack.at(-1)?.content||root).append(section);setOpen(section,true);stack.push({level,content});
        }else (stack.at(-1)?.content||root).append(node);
      }
    }
  }
  function reveal(target){
    for(let p=target;p;p=p.parentElement){if(p.classList?.contains('desk-section'))setOpen(p,true);if(p.tagName==='DETAILS')p.open=true;}
  }
  document.addEventListener('click',e=>{
    const b=e.target.closest('.desk-section-toggle');if(b){e.preventDefault();e.stopPropagation();setOpen(b.closest('.desk-section'),b.getAttribute('aria-expanded')!=='true');syncOutline();return;}
    const a=e.target.closest('a[href^="#"]');if(!a)return;
    let id;try{id=decodeURIComponent(a.getAttribute('href').slice(1));}catch{return;}const target=document.getElementById(id);if(target)reveal(target);
  },true);
  let pending=false;
  function syncOutline(){if(pending)return;pending=true;requestAnimationFrame(()=>{
    pending=false;const root=document.getElementById('post'),links=[...document.querySelectorAll('#outline-list a[href^="#"]')];if(!root||!links.length)return;
    const visible=links.map(a=>{let h;try{h=document.getElementById(decodeURIComponent(a.getAttribute('href').slice(1)));}catch{}return {a,h};}).filter(x=>x.h&&x.h.getClientRects().length);
    const top=root.getBoundingClientRect().top+30;let active=visible[0];for(const item of visible)if(item.h.getBoundingClientRect().top<=top)active=item;
    links.forEach(a=>a.classList.toggle('active',a===active?.a));
  });}
  function ready(){install();const target=()=>{let id;try{id=decodeURIComponent(location.hash.slice(1));}catch{return;}const h=document.getElementById(id);if(h)reveal(h);};target();window.addEventListener('hashchange',target);document.getElementById('post')?.addEventListener('scroll',syncOutline,{passive:true});if(window.jQuery)window.jQuery(document).on('pjax:end',()=>{install();target();syncOutline();});}
  if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',ready);else ready();
})();
