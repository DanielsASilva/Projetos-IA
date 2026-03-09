# Ontologia: Entrega de Trabalhos Acadêmicos

## Classes

- Aluno
- Professor
- Tema
- Grupo
- Entregável

## Relações

- Aluno pertence a um grupo
    - pertence (Aluno -> Grupo)
- Grupo entrega o entregável para o professor
    - entrega (Grupo -> Entregável)
    - recebe (Professor -> Entregável)
- Grupo tem um tema
    - contem (Grupo -> Tema)
- Aluno ou Professor cria Grupo
    - cria (Aluno -> Grupo)
    - cria (Professor -> Grupo)

## Propriedades

- Aluno: matricula, nome, cpf
- Professor: inscricao, nome
- Tema: nome
- Grupo: numero
- Entregável: nome, conteudo

- Aluno -> pertence -> Grupo
- Grupo -> entrega -> Entregavel
- Professor -> recebe -> Entregável
- Grupo -> contem -> Tema

## Restrições Semânticas

- Todo Grupo deve entregar um Entregável
- Todo Aluno deve pertencer a um grupo
- Um Grupo deve ter pelo menos um Aluno
- Um Grupo deve ter pelo menos um Tema

