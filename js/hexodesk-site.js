(function(){
  function themeButton(){
    const b=document.getElementById('desk-theme-toggle');if(!b)return;
    const dark=document.documentElement.dataset.blogTheme!=='light';b.innerHTML='<svg viewBox="0 0 24 24" aria-hidden="true">'+(dark?'<circle cx="12" cy="12" r="4"/><path d="M12 2v2m0 16v2M2 12h2m16 0h2M5 5l1.5 1.5m11 11L19 19M5 19l1.5-1.5m11-11L19 5"/>':'<path d="M20 15.5A8.5 8.5 0 0 1 8.5 4 8.5 8.5 0 1 0 20 15.5z"/>')+'</svg>';b.setAttribute('aria-label',dark?'切换到亮色模式':'切换到暗色模式');b.title=b.getAttribute('aria-label');
  }
  document.addEventListener('click',e=>{
    if(e.target.closest('#desk-scroll-top')){document.getElementById('post')?.scrollTo({top:0,behavior:matchMedia('(prefers-reduced-motion: reduce)').matches?'auto':'smooth'});return;}
    // Home owns its filter history; let the avatar use a normal navigation instead of old PJAX cache state.
    if(e.target.closest('.avatar_target')){e.stopPropagation();return;}
    const hashLink=e.target.closest('a[href^="#"]');if(hashLink)revealHash(hashLink.getAttribute('href'));
    if(e.target.closest('#desk-theme-toggle')){
      const mode=document.documentElement.dataset.blogTheme==='light'?'dark':'light';
      document.documentElement.dataset.blogTheme=mode;document.documentElement.style.colorScheme=mode;
      try{localStorage.setItem('hexodesk:blog-theme',mode);}catch{}themeButton();
    }
    const home=document.querySelector('[data-desk-home]'),link=e.target.closest('[data-desk-view],[data-desk-year],[data-desk-tag],[data-desk-clear]');
    if(!home||!link||e.button!==0||e.ctrlKey||e.metaKey||e.shiftKey||e.altKey)return;
    e.preventDefault();e.stopPropagation();const url=new URL(location.href);url.hash='';
    if(link.hasAttribute('data-desk-clear'))url.search='';
    if(link.hasAttribute('data-desk-view')){const v=link.dataset.deskView;if(v==='tags')url.searchParams.set('view','tags');else url.searchParams.delete('view');}
    if(link.hasAttribute('data-desk-year')){const v=link.dataset.deskYear;if(v)url.searchParams.set('year',v);else url.searchParams.delete('year');}
    if(link.hasAttribute('data-desk-tag')){url.searchParams.set('tag',link.dataset.deskTag);url.searchParams.delete('view');}
    history.pushState({},'',url);applyHome();
  },true);
  function applyHome(){
    const home=document.querySelector('[data-desk-home]');if(!home)return;
    const params=new URL(location.href).searchParams,view=params.get('view')==='tags'?'tags':'timeline',year=params.get('year')||'',tag=params.get('tag')||'',q=params.get('q')||'';
    home.querySelector('.desk-tags-view').hidden=view!=='tags';home.querySelector('.desk-timeline-view').hidden=view==='tags';
    home.querySelectorAll('[data-desk-view]').forEach(a=>{if(a.dataset.deskView===view)a.setAttribute('aria-current','page');else a.removeAttribute('aria-current');});
    home.querySelectorAll('[data-desk-year]').forEach(a=>{if(a.dataset.deskYear===year)a.setAttribute('aria-current','true');else a.removeAttribute('aria-current');});
    const search=home.querySelector('[data-desk-title-search]');if(document.activeElement!==search)search.value=q;
    let count=0;home.querySelectorAll('.desk-timeline-entry').forEach(li=>{const match=(!year||li.dataset.year===year)&&(!tag||JSON.parse(li.dataset.tags).includes(tag))&&li.dataset.deskTitle.toLocaleLowerCase().includes(q.toLocaleLowerCase());li.hidden=!match;if(match)count++;});
    home.querySelectorAll('.desk-timeline-group').forEach(group=>group.hidden=![...group.querySelectorAll('li')].some(li=>!li.hidden));
    home.querySelector('[data-desk-result-count]').textContent=count+' 篇';home.querySelector('.desk-empty-results').hidden=count>0;
    home.querySelector('.desk-filter-bar').hidden=!year&&!tag&&!q;home.querySelector('[data-desk-filter-label]').textContent=[year&&year+' 年',tag&&'#'+tag,q&&'标题：'+q].filter(Boolean).join(' · ');
  }
  document.addEventListener('input',e=>{
    if(e.target.matches('[data-desk-title-search]')){const url=new URL(location.href),q=e.target.value;if(q)url.searchParams.set('q',q);else url.searchParams.delete('q');history.replaceState({},'',url);applyHome();}
    if(e.target.matches('[data-desk-tag-search]')){const section=e.target.closest('.desk-tags-view'),q=e.target.value.trim().toLocaleLowerCase();let count=0;section.querySelectorAll('[data-desk-tag]').forEach(a=>{a.hidden=!a.dataset.deskTag.toLocaleLowerCase().includes(q);if(!a.hidden)count++;});section.querySelector('.desk-no-tags').hidden=count>0;}
  });
  window.addEventListener('popstate',applyHome);
  function revealHash(hash=location.hash){
    let id;try{id=decodeURIComponent(hash.slice(1));}catch{return;}if(!id)return;
    const target=document.getElementById(id);if(!target)return;
    for(let p=target.parentElement;p;p=p.parentElement)if(p.tagName==='DETAILS')p.open=true;
  }
  window.addEventListener('hashchange',()=>revealHash());
  document.addEventListener('keydown',e=>{if(e.target.matches('[data-desk-title-search],[data-desk-tag-search]'))e.stopPropagation();},true);
  function updateTools(){const b=document.getElementById('desk-scroll-top');if(b)b.disabled=(document.getElementById('post')?.scrollTop||0)<10;}
  function ready(){themeButton();applyHome();revealHash();updateTools();document.getElementById('post')?.addEventListener('scroll',updateTools,{passive:true});if(window.jQuery)window.jQuery(document).on('pjax:end',()=>{applyHome();revealHash();updateTools();});}
  if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',ready);else ready();
})();
