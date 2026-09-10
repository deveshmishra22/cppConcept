/* Enterprise C++ Handbook — Search Implementation
   Ctrl+K or clicking the search bar opens a modal.
   Results filter as you type. Click a result to navigate.
   Works on both dark (module0/1) and light (module2+) themes
   via CSS custom properties shared by both themes.
*/

(function () {
  // ─── Search index ──────────────────────────────────────────────
  var INDEX = [
    // Landing page
    { title: "Course Overview", sub: "All modules · 6 days", url: "index.html", tab: null,
      body: "enterprise cpp handbook foundations oop polymorphism robustness modern networking" },

    // Module 0
    { title: "Foundation Enhancer", sub: "Module 0 · Day 1", url: "module0.html", tab: null,
      body: "why cpp exists c to cpp migration procedural oop paradigm struct class malloc free new delete return codes exceptions toolchain setup g++ compiler" },
    { title: "Evolution of C to C++", sub: "Module 0 · Topic 01", url: "module0.html#t1", tab: "t1",
      body: "c language history bjarne stroustrup object oriented classes structs memory management why cpp replaced c procedural programming" },
    { title: "Procedural vs OOP", sub: "Module 0 · Topic 02", url: "module0.html#t2", tab: "t2",
      body: "procedural paradigm oop paradigm data behaviour encapsulation object state method procedure function" },
    { title: "Key C to C++ Differences", sub: "Module 0 · Topic 03", url: "module0.html#t3", tab: "t3",
      body: "struct class malloc free new delete return codes exceptions references namespaces std cout cin endl" },

    // Module 1
    { title: "C++ Basics", sub: "Module 1 · Day 1", url: "module1.html", tab: null,
      body: "classes objects access specifiers constructors destructors static members friend functions bank account case study" },
    { title: "Classes & Objects", sub: "Module 1 · Topic 01", url: "module1.html#t1", tab: "t1",
      body: "class keyword object instance stack allocation heap allocation new delete member function data member BankAccount" },
    { title: "Access Specifiers", sub: "Module 1 · Topic 02", url: "module1.html#t2", tab: "t2",
      body: "public private protected encapsulation data hiding getter setter balance validation access control" },
    { title: "Constructors & Destructors", sub: "Module 1 · Topic 03", url: "module1.html#t3", tab: "t3",
      body: "default constructor parameterized copy constructor destructor initializer list uninitialised state resource cleanup" },
    { title: "Static Members", sub: "Module 1 · Topic 04", url: "module1.html#t4", tab: "t4",
      body: "static data member static method class-level variable shared across instances total accounts bank" },
    { title: "Friend Functions", sub: "Module 1 · Topic 05", url: "module1.html#t5", tab: "t5",
      body: "friend function friend class access private members audit comparison transfer between accounts" },

    // Module 2
    { title: "Inheritance & Polymorphism", sub: "Module 2 · Day 2", url: "module2.html", tab: null,
      body: "inheritance virtual functions abstract classes destructors vtable shape hierarchy runtime polymorphism" },
    { title: "Inheritance", sub: "Module 2 · Topic 01", url: "module2.html#t1", tab: "t1",
      body: "base class derived class public inheritance colon syntax is-a relationship code reuse Shape Circle Rectangle constructor chaining Vehicle Car" },
    { title: "Virtual Functions", sub: "Module 2 · Topic 02", url: "module2.html#t2", tab: "t2",
      body: "virtual keyword override dynamic dispatch runtime binding vtable vptr static binding draw shape polymorphism C# comparison" },
    { title: "Abstract Classes", sub: "Module 2 · Topic 03", url: "module2.html#t3", tab: "t3",
      body: "pure virtual function abstract class interface contract cannot instantiate area name shape circle rectangle compile-time enforcement" },
    { title: "Virtual Destructors & vtables", sub: "Module 2 · Topic 04", url: "module2.html#t4", tab: "t4",
      body: "virtual destructor memory leak resource cleanup vtable vptr pointer delete base derived ManagedBuffer Resource" },

    // Module 3
    { title: "Compile-time Polymorphism", sub: "Module 3 · Day 2", url: "module3.html", tab: null,
      body: "function overloading operator overloading complex numbers point comparison compile-time dispatch" },
    { title: "Function Overloading", sub: "Module 3 · Topic 01", url: "module3.html#t1", tab: "t1",
      body: "overload resolution same name different parameters types int double string add overloading compile-time selection" },
    { title: "Operator Overloading", sub: "Module 3 · Topic 02", url: "module3.html#t2", tab: "t2",
      body: "operator+ operator== operator!= operator< operator<< member function free function vector2d value type" },
    { title: "Complex Numbers", sub: "Module 3 · Topic 03", url: "module3.html#t3", tab: "t3",
      body: "complex number real imaginary addition multiplication operator overloading value type component-wise arithmetic" },
    { title: "Point Comparison", sub: "Module 3 · Topic 04", url: "module3.html#t4", tab: "t4",
      body: "point equality floating point tolerance epsilon operator== operator!= operator< checkpoint player game coordinate comparison" },

    // Module 4
    { title: "Exception Handling & Robustness", sub: "Module 4 · Day 3", url: "module4.html", tab: null,
      body: "try catch throw exception RAII runtime_error custom exception robust payment processor error handling" },
    { title: "try/catch basics", sub: "Module 4 · Topic 01", url: "module4.html#t1", tab: "t1",
      body: "try block throw statement catch clause stack unwinding std::runtime_error return codes error code trap divide by zero program survives" },
    { title: "Catch by type", sub: "Module 4 · Topic 02", url: "module4.html#t2", tab: "t2",
      body: "multiple catch clauses std::exception hierarchy std::invalid_argument std::out_of_range catch order what() typed exceptions context" },
    { title: "Custom Exceptions", sub: "Module 4 · Topic 03", url: "module4.html#t3", tab: "t3",
      body: "custom exception class inherit std::exception what() noexcept override domain context InsufficientFundsError typed members account balance" },
    { title: "RAII", sub: "Module 4 · Topic 04", url: "module4.html#t4", tab: "t4",
      body: "RAII resource acquisition initialization constructor destructor file handle ifstream ofstream lock_guard unique_ptr exception safe FileReader always closes" },

    // Module 5
    { title: "Templates & Generics", sub: "Module 5 · Day 3", url: "module5.html", tab: null,
      body: "function template class template STL standard template library generic programming type parameter typename" },
    { title: "Why Templates", sub: "Module 5 · Topic 01", url: "module5.html#t1", tab: "t1",
      body: "code duplication problem type parameter compile-time code generation zero runtime overhead template instantiation maxOf typename vs class header file" },
    { title: "Function Templates", sub: "Module 5 · Topic 02", url: "module5.html#t2", tab: "t2",
      body: "template<typename T> type deduction const T& explicit template argument swap clamp non-type parameter void* type safety" },
    { title: "Class Templates", sub: "Module 5 · Topic 03", url: "module5.html#t3", tab: "t3",
      body: "class template Stack<T> member function outside definition distinct instantiation multiple type parameters vector int string" },
    { title: "STL as Templates", sub: "Module 5 · Topic 04", url: "module5.html#t4", tab: "t4",
      body: "std::vector std::map std::unordered_map std::optional std::pair std::unique_ptr std::sort std::find_if standard template library container algorithm" },

    // Capstone exercises
    { title: "Capstone: RR Skillverse Course Tracker", sub: "Practice lab", url: "exercise.html", tab: null,
      body: "capstone exercise course tracker classes inheritance virtual dispatch operator overloading complete program" },
    { title: "Capstone: Smart Office Access System", sub: "Practice lab", url: "capstone-work1.html", tab: null,
      body: "smart office badge access employee contractor visitor polymorphism capstone exercise" },
    { title: "Capstone-Work2: Nimbus Logistics Challenge", sub: "Practice lab", url: "capstone-work2.html", tab: null,
      body: "nimbus logistics challenge mode templates stl exceptions inheritance overloading repository generic order fulfillment unfinished features todo capstone" },
  ];

  // ─── Score a result against a query ─────────────────────────────
  function score(entry, query) {
    var q = query.toLowerCase();
    var title = entry.title.toLowerCase();
    var sub   = entry.sub.toLowerCase();
    var body  = entry.body.toLowerCase();
    if (title.includes(q)) return 3;
    if (sub.includes(q))   return 2;
    // token search in body
    var tokens = q.split(/\s+/).filter(Boolean);
    var matched = tokens.filter(function(t) { return body.includes(t); });
    if (matched.length === tokens.length) return 1;
    if (matched.length > 0) return 0.5;
    return 0;
  }

  // ─── Navigate to a search result ────────────────────────────────
  function navigate(entry) {
    if (entry.tab) {
      // Navigate to the page, then activate the tab via hash
      var current = location.pathname.split('/').pop();
      var target  = entry.url.split('#')[0];
      if (current === target || (current === '' && target === 'index.html')) {
        // Same page — just activate the tab
        var hash = entry.url.split('#')[1];
        if (hash) {
          var btn = document.querySelector('[data-tab="' + hash + '"]');
          if (btn) btn.click();
          location.hash = hash;
        }
      } else {
        location.href = entry.url;
      }
    } else {
      location.href = entry.url;
    }
    closeModal();
  }

  // ─── Render results ──────────────────────────────────────────────
  function renderResults(query, list, container) {
    container.innerHTML = '';
    if (!query || query.length < 2) {
      container.innerHTML = '<div class="sr-empty">Type at least 2 characters to search…</div>';
      return;
    }
    var scored = INDEX.map(function(e) {
      return { entry: e, s: score(e, query) };
    }).filter(function(x) { return x.s > 0; })
      .sort(function(a, b) { return b.s - a.s; })
      .slice(0, 12);

    if (scored.length === 0) {
      container.innerHTML = '<div class="sr-empty">No results for "<strong>' + escHtml(query) + '</strong>"</div>';
      return;
    }
    scored.forEach(function(x, i) {
      var e = x.entry;
      var el = document.createElement('button');
      el.className = 'sr-item' + (i === 0 ? ' sr-item--active' : '');
      el.type = 'button';
      el.innerHTML =
        '<span class="sr-title">' + escHtml(e.title) + '</span>' +
        '<span class="sr-sub">' + escHtml(e.sub) + '</span>';
      el.addEventListener('click', function() { navigate(e); });
      list.appendChild(el);
    });
  }

  function escHtml(s) {
    return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
  }

  // ─── Keyboard navigation within results ─────────────────────────
  function moveFocus(list, dir) {
    var items = list.querySelectorAll('.sr-item');
    if (!items.length) return;
    var active = list.querySelector('.sr-item--active');
    var idx = -1;
    items.forEach(function(el, i) { if (el === active) idx = i; });
    if (active) active.classList.remove('sr-item--active');
    idx = (idx + dir + items.length) % items.length;
    items[idx].classList.add('sr-item--active');
    items[idx].scrollIntoView({ block: 'nearest' });
  }

  // ─── Modal open / close ──────────────────────────────────────────
  var overlay, modal, input, list;

  function openModal() {
    if (overlay) { overlay.style.display = 'flex'; input.focus(); input.select(); return; }
    injectStyles();
    overlay = document.createElement('div');
    overlay.className = 'sr-overlay';
    overlay.setAttribute('role', 'dialog');
    overlay.setAttribute('aria-modal', 'true');
    overlay.setAttribute('aria-label', 'Search the handbook');

    modal = document.createElement('div');
    modal.className = 'sr-modal';

    var searchRow = document.createElement('div');
    searchRow.className = 'sr-searchrow';

    var icon = document.createElement('span');
    icon.className = 'sr-icon';
    icon.textContent = '⌕';

    input = document.createElement('input');
    input.type = 'search';
    input.className = 'sr-input';
    input.placeholder = 'Search modules, topics, concepts…';
    input.autocomplete = 'off';
    input.spellcheck = false;

    var closeBtn = document.createElement('button');
    closeBtn.className = 'sr-close';
    closeBtn.type = 'button';
    closeBtn.setAttribute('aria-label', 'Close search');
    closeBtn.textContent = '✕';
    closeBtn.addEventListener('click', closeModal);

    searchRow.appendChild(icon);
    searchRow.appendChild(input);
    searchRow.appendChild(closeBtn);

    list = document.createElement('div');
    list.className = 'sr-list';
    list.innerHTML = '<div class="sr-empty">Type at least 2 characters to search…</div>';

    var footer = document.createElement('div');
    footer.className = 'sr-footer';
    footer.innerHTML = '<span>↑↓ navigate</span><span>↵ open</span><span>Esc close</span>';

    modal.appendChild(searchRow);
    modal.appendChild(list);
    modal.appendChild(footer);
    overlay.appendChild(modal);
    document.body.appendChild(overlay);

    input.addEventListener('input', function() {
      renderResults(input.value.trim(), list, list);
    });

    input.addEventListener('keydown', function(e) {
      if (e.key === 'ArrowDown')  { e.preventDefault(); moveFocus(list, 1); }
      if (e.key === 'ArrowUp')    { e.preventDefault(); moveFocus(list, -1); }
      if (e.key === 'Escape')     { closeModal(); }
      if (e.key === 'Enter') {
        var active = list.querySelector('.sr-item--active');
        if (active) active.click();
      }
    });

    overlay.addEventListener('click', function(e) {
      if (e.target === overlay) closeModal();
    });

    requestAnimationFrame(function() { input.focus(); });
  }

  function closeModal() {
    if (overlay) overlay.style.display = 'none';
  }

  // ─── Wire up the existing search bar on the page ─────────────────
  function wireSearchBar() {
    var bars = document.querySelectorAll('.search-input');
    bars.forEach(function(bar) {
      bar.addEventListener('focus', function(e) {
        e.preventDefault();
        bar.blur();
        openModal();
      });
      bar.parentElement.addEventListener('click', function(e) {
        e.preventDefault();
        openModal();
      });
    });
  }

  // ─── Global keyboard shortcut Ctrl+K / Cmd+K ─────────────────────
  document.addEventListener('keydown', function(e) {
    if ((e.ctrlKey || e.metaKey) && e.key === 'k') {
      e.preventDefault();
      openModal();
    }
    if (e.key === 'Escape' && overlay && overlay.style.display !== 'none') {
      closeModal();
    }
  });

  // ─── Inject CSS ───────────────────────────────────────────────────
  function injectStyles() {
    if (document.getElementById('sr-styles')) return;
    var s = document.createElement('style');
    s.id = 'sr-styles';
    // Uses CSS custom properties that exist on both dark and light theme pages.
    // Falls back to reasonable values if a variable isn't set.
    s.textContent = [
      '.sr-overlay{',
        'position:fixed;inset:0;z-index:9999;',
        'display:flex;align-items:flex-start;justify-content:center;',
        'padding-top:80px;',
        'background:rgba(0,0,0,0.55);',
        'backdrop-filter:blur(3px);',
      '}',
      '.sr-modal{',
        'width:min(680px,calc(100vw - 32px));',
        'background:var(--surface,#fff);',
        'border:1px solid var(--border,#e5e7eb);',
        'border-radius:14px;',
        'box-shadow:0 24px 64px rgba(0,0,0,0.35);',
        'overflow:hidden;',
        'display:flex;flex-direction:column;',
        'max-height:calc(100vh - 140px);',
      '}',
      '.sr-searchrow{',
        'display:flex;align-items:center;gap:12px;',
        'padding:16px 18px;',
        'border-bottom:1px solid var(--border,#e5e7eb);',
        'flex-shrink:0;',
      '}',
      '.sr-icon{font-size:20px;color:var(--text-faint,#94a3b8);flex-shrink:0;}',
      '.sr-input{',
        'flex:1;border:none;outline:none;',
        'background:transparent;',
        'font-size:16px;color:var(--text,#1e293b);',
        'font-family:inherit;',
      '}',
      '.sr-input::placeholder{color:var(--text-faint,#94a3b8);}',
      '.sr-close{',
        'background:none;border:none;cursor:pointer;',
        'color:var(--text-faint,#94a3b8);font-size:16px;padding:4px 6px;',
        'border-radius:6px;flex-shrink:0;',
      '}',
      '.sr-close:hover{color:var(--text,#1e293b);background:var(--surface-2,#f1f5f9);}',
      '.sr-list{overflow-y:auto;flex:1;padding:8px;}',
      '.sr-item{',
        'width:100%;text-align:left;',
        'display:flex;flex-direction:column;gap:2px;',
        'padding:10px 14px;border-radius:8px;border:none;',
        'cursor:pointer;background:transparent;',
        'color:var(--text,#1e293b);',
      '}',
      '.sr-item:hover,.sr-item--active{',
        'background:var(--surface-2,#f1f5f9);',
      '}',
      '.sr-title{font-size:14px;font-weight:600;color:var(--text,#1e293b);}',
      '.sr-sub{font-size:12px;color:var(--text-faint,#94a3b8);font-family:monospace;}',
      '.sr-empty{',
        'padding:24px 14px;text-align:center;',
        'color:var(--text-faint,#94a3b8);font-size:14px;',
      '}',
      '.sr-footer{',
        'display:flex;gap:20px;justify-content:center;',
        'padding:10px 18px;',
        'border-top:1px solid var(--border,#e5e7eb);',
        'font-size:12px;color:var(--text-faint,#94a3b8);',
        'flex-shrink:0;',
      '}',
    ].join('');
    document.head.appendChild(s);
  }

  // ─── Init ─────────────────────────────────────────────────────────
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', wireSearchBar);
  } else {
    wireSearchBar();
  }
})();
