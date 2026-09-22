menu = {"pizza": 3.00,
               "nachos": 4.50,
               "popcorn": 6.00,
               "fries": 2.50,
               "chips": 1.00,
               "pretzel": 3.50,
               "soda": 3.00,
               "lemonade": 4.25}
cart = []
total = 0
print("----welcome!----")
for key,value in menu.items() :
    print(f"{key:10} : ${value:.2f}$ ")
    print("-------------------")
while True: 
        item = input("what would you like to order?(press q to quit): ").lower()

        if item == "q" :
            break
        elif menu.get(item) is not None :
            cart.append(item)
        else :
            print("sorry!unavalable item")
for food in cart :
    total += menu[food]
    print(food,end =" ")

print()
print((f"your total is : {total :.2f}taka"))