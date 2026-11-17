(function(){
  const runtime=new URL('vendor/mermaid/mermaid.esm.min.js',document.currentScript.src).href;
  const rendered=new WeakMap();let library,serial=0,busy=false,requested=false;
  const mode=()=>document.documentElement.dataset.blogTheme==='light'?'light':'dark';
  function configuration(){
    const css=getComputedStyle(document.documentElement),color=name=>css.getPropertyValue('--site-'+name).trim();
    return {startOnLoad:false,securityLevel:'strict',suppressErrorRendering:true,theme:'base',look:'classic',layout:'dagre',htmlLabels:false,darkMode:mode()==='dark',fontFamily:'system-ui, "Microsoft YaHei", sans-serif',
      themeVariables:{darkMode:mode()==='dark',background:color('panel'),primaryColor:color('quote'),primaryTextColor:color('text'),primaryBorderColor:color('accent'),lineColor:color('muted'),secondaryColor:color('hover'),tertiaryColor:color('panel'),textColor:color('text'),nodeTextColor:color('text'),edgeLabelBackground:color('panel'),clusterBkg:color('quote'),clusterBorder:color('line')},flowchart:{htmlLabels:false,wrappingWidth:240}};
  }
  async function refresh(){
    requested=true;if(busy)return;busy=true;
    try{
      while(requested){
        requested=false;const theme=mode(),blocks=[...document.querySelectorAll('#post .article-entry .desk-mermaid')].filter(block=>rendered.get(block)!==theme);
        if(!blocks.length)continue;
        let mermaid;
        try{mermaid=(await (library??=import(runtime))).default;}catch{library=null;}
        if(mermaid)mermaid.initialize(configuration());
        for(const block of blocks){
          const source=block.querySelector('.desk-mermaid-source'),output=block.querySelector('.desk-mermaid-output'),error=block.querySelector('.desk-mermaid-error');
          if(!source||!output||!error||!block.isConnected)continue;
          try{
            if(!mermaid)throw Error('load');
            const {svg}=await mermaid.render('desk-mermaid-'+(++serial),source.textContent);
            if(mode()!==theme){requested=true;break;}
            if(!block.isConnected)continue;
            output.innerHTML=svg;output.hidden=false;source.hidden=true;error.hidden=true;
            output.querySelector('svg')?.setAttribute('aria-label','Mermaid 图表');
          }catch{
            if(!block.isConnected)continue;
            output.replaceChildren();output.hidden=true;source.hidden=false;error.hidden=false;
            error.textContent=mermaid?'图表语法有误，请检查 Mermaid 源码。':'图表加载失败，请刷新重试。';
          }
          rendered.set(block,theme);
        }
      }
    }finally{busy=false;}
  }
  function ready(){
    refresh();
    new MutationObserver(refresh).observe(document.documentElement,{attributes:true,attributeFilter:['data-blog-theme']});
    const post=document.getElementById('post');
    if(post)new MutationObserver(records=>{
      if(records.some(record=>[...record.addedNodes].some(node=>node.nodeType===1&&(node.matches('.desk-mermaid')||node.querySelector('.desk-mermaid')))))refresh();
    }).observe(post,{childList:true,subtree:true});
    if(window.jQuery)window.jQuery(document).on('pjax:end',refresh);
    document.addEventListener('toggle',event=>{if(event.target.tagName==='DETAILS'&&event.target.open)refresh();},true);
  }
  if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',ready);else ready();
})();
