#include <iostream>
#include <cassert>
using namespace std;
/*
* Referência inteligente
*/
template<typename Valor>
struct Inteligente
{
private:
template<typename OutroValor>
friend struct Inteligente;
Valor* ponteiro;
public:
Inteligente():ponteiro{nullptr}
{
criador=&cria<Valor>;
}
template<typename OutroValor>
Inteligente (const OutroValor& outro)
{
criador=&cria<OutroValor>;
ponteiro=criador(outro);
}
Inteligente (const Inteligente& outro)
{
criador=outro.criador;
ponteiro=criador(*(outro.ponteiro));
}
Inteligente (Inteligente&& outro):
ponteiro(outro.ponteiro),criador(outro.criador)
{
outro.ponteiro=nullptr;
}
template<typename OutroValor>
Inteligente (const Inteligente<OutroValor>& outro)
{
criador=outro.criador;
ponteiro=criador(*(outro.ponteiro));
}
template<typename OutroValor>
Inteligente (Inteligente<OutroValor>&& outro):
ponteiro(outro.ponteiro),criador(outro.criador)
{
outro.ponteiro=nullptr;
}
template<typename OutroValor>
Inteligente&
operator= (const Inteligente<OutroValor>& outro)
{
if(&outro!=this)
{
delete ponteiro;
criador=outro.criador;
ponteiro=criador(*(outro.ponteiro));
}
return *this;
}
Inteligente&
operator= (const Inteligente& outro)
{
if(&outro!=this)
{
delete ponteiro;
criador=outro.criador;
ponteiro=criador(*(outro.ponteiro));
}
return *this;
}
template<typename OutroValor>
Inteligente&
operator= (Inteligente<OutroValor>&& outro)
{
if(&outro!=this)
{
delete ponteiro;
criador=outro.criador;
ponteiro=outro.ponteiro;
outro.ponteiro=nullptr;
}
return *this;
}
Inteligente&
operator= (const Valor& outro)
{
if (ponteiro)
{
*ponteiro=outro;
}
else
{
ponteiro=criador(outro) ;
}
return *this;
}
~Inteligente()
{
delete ponteiro;
ponteiro=0;
}
Valor&
operator()()
{
assert (ponteiro);
return *ponteiro;
}
const Valor&
operator()() const
{
assert (ponteiro);
return *ponteiro;
}
operator Valor& ()
{
assert (ponteiro);
return *ponteiro;
}
operator const Valor& () const
{
assert (ponteiro);
return *ponteiro;
}
template<typename OutroValor>
OutroValor& como()
{
return (OutroValor&) this->operator()();
}
template<typename OutroValor>
const OutroValor& como() const
{
return (OutroValor&) this->operator()();
}
operator bool () const
{
return ponteiro;
}
private:
template<typename OutroValor>
static Valor * cria(const Valor& arg)
{
return new OutroValor((OutroValor&)arg);
}
typedef Valor* (*CRIACAO)(const Valor& arg);
CRIACAO criador=nullptr;
public:
template<typename OutroValor,typename ...Argumentos>
static Inteligente
fazInteligente (const Argumentos& ... argumentos)
{
return Inteligente (OutroValor (argumentos ...));
}
};
