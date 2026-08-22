document.querySelectorAll('.cd p.desc').forEach(p=>{
 const b=document.createElement('button');b.type='button';b.className='more';b.textContent='Read more';
 p.after(b);
 b.onclick=()=>{const c=p.closest('.cd');const o=c.classList.toggle('open');b.textContent=o?'Show less':'Read more'};
 if(p.scrollHeight<=p.clientHeight+2)b.remove();
});
