// using System;
// using System.Collections.Generic;

// namespace GoingDown 
// {
//     public abstract class Character
//     {
//         public string Name { get; protected set; }
//         public int HP { get; protected set; }
//         public int MaxHP { get; protected set; }
//         public int MP { get; protected set; }
//         public int MaxMP { get; protected set; }
//         public int Strength { get; protected set; }
//         public int Constitution { get; protected set; }
//         public int AC { get; protected set; } // Armor Class
//         public Dictionary<string, int> StatusEffects { get; private set; } = new Dictionary<string, int>();

//         protected Character(string name, int hp, int mp, int strength, int constitution, int ac)
//         {
//             Name = name;
//             MaxHP = hp;
//             HP = hp;
//             MaxMP = mp;
//             MP = mp;
//             Strength = strength;
//             Constitution = constitution;
//             AC = ac;
//         }

//         public bool IsAlive => HP > 0;

//         public virtual void TakeDamage(int damage)
//         {
//             HP -= damage;
//             if (HP < 0) HP = 0;
//         }

//         public void ApplyStatusEffect(string effect, int duration)
//         {
//             if (StatusEffects.ContainsKey(effect))
//                 StatusEffects[effect] += duration;
//             else
//                 StatusEffects[effect] = duration;
//         }

//         public void UpdateStatusEffects()
//         {
//             var expiredEffects = new List<string>();
//             foreach (var effect in StatusEffects.Keys)
//             {
//                 StatusEffects[effect]--;
//                 if (StatusEffects[effect] <= 0)
//                     expiredEffects.Add(effect);
//             }
//             foreach (var effect in expiredEffects)
//                 StatusEffects.Remove(effect);
//         }

//         public virtual void EndTurn()
//         {
//             UpdateStatusEffects();
//         }
//     }
// }

