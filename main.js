const nav=document.querySelector('nav.pill'),hero=document.querySelector('.hero');
if(nav&&hero){const sync=()=>nav.classList.toggle('solid',hero.getBoundingClientRect().bottom<nav.getBoundingClientRect().bottom+8);
addEventListener('scroll',sync,{passive:true});addEventListener('resize',sync);sync();}
document.querySelectorAll('.row p').forEach(p=>{
 const b=document.createElement('button');b.type='button';b.className='more';b.textContent='Read more';
 const repo=p.parentElement.querySelector('.repo');
 repo?repo.before(b):p.after(b);
 b.onclick=()=>{const o=p.closest('.row').classList.toggle('open');b.textContent=o?'Show less':'Read more'};
 if(p.scrollHeight<=p.clientHeight+2)b.remove();
});
