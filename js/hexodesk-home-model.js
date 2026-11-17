(function(root,factory){if(typeof module==='object'&&module.exports)module.exports=factory();else root.DeskHome=factory();})(typeof globalThis!=='undefined'?globalThis:this,function(){
  function tagsFrom(params){return [...new Set(params.getAll('tag').filter(Boolean))];}
  function toggleTag(url,tag){const selected=tagsFrom(url.searchParams),next=selected.includes(tag)?selected.filter(value=>value!==tag):[...selected,tag];url.searchParams.delete('tag');for(const value of next)url.searchParams.append('tag',value);url.searchParams.set('view','tags');return url;}
  function viewFrom(params){const view=params.get('view');return ['timeline','tags','stats'].includes(view)?view:params.has('tag')?'tags':'timeline';}
  function filtersFrom(params){const view=viewFrom(params);return {view,year:view==='timeline'?params.get('year')||'':'',q:view==='timeline'?params.get('q')||'':'',tags:view==='tags'?tagsFrom(params):[]};}
  function clearFilters(url){const view=viewFrom(url.searchParams);if(view==='tags')url.searchParams.delete('tag');else if(view==='timeline'){url.searchParams.delete('year');url.searchParams.delete('q');}return url;}
  function matches(post,{year='',tags=[],q=''}={}){return (!year||post.year===year)&&(!tags.length||tags.some(tag=>post.tags.includes(tag)))&&post.title.toLocaleLowerCase().includes(q.toLocaleLowerCase());}
  function sortByWords(posts,order='words-desc'){const direction=order==='words-asc'?1:-1;return [...posts].sort((a,b)=>direction*((a.words||0)-(b.words||0))||(b.timestamp||0)-(a.timestamp||0)||a.title.localeCompare(b.title,'zh-CN'));}
  function parseVisits(text){const value=String(text).replace(/[,\s]/g,'');if(!/^\d+$/.test(value))return null;const count=Number(value);return Number.isSafeInteger(count)?count:null;}
  return {tagsFrom,toggleTag,viewFrom,filtersFrom,clearFilters,matches,sortByWords,parseVisits};
});
