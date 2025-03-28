
class RelaxClass:

    def __init__(self, name: str = "", fields: list[str] = []):
        self.name = name
        self.fields = fields

    def to_string(self):
        return f"Class name: {self.name}, Class fields: {self.fields}"
    
relaxClasses = []

def get_class(class_name: str):
    for clazz in relaxClasses:
        if clazz.name == class_name:
            return clazz
    return None

def class_has_field(clazz: RelaxClass, field_name: str):
    for field in clazz.fields:
        if field == field_name:
            return True
        
    return False

def create_class(parts: list[str]):
    name = parts[1]
    fields = []
    for part in parts[2:]:
        if part.startswith("@"):
            referenceClass: RelaxClass = get_class(part.split(".")[0].removeprefix("@"))
            print("Checking reference class: ", part.split(".")[0].removeprefix("@"), " field: ", part.split(".")[1])
            if referenceClass and class_has_field(referenceClass, part.split(".")[1]):
                pass
            else:
                print("Reference class or field doesnt exist")
        fields.append(part)
    clazz = RelaxClass(name, fields)
    relaxClasses.append(clazz)

def interpret(code):
    parts = code.split()
    
    if parts[0] == "class":
        create_class(parts)

interpret("class Engine id brand weight")
interpret("class Car id name color @Engine.id")
interpret("create Engine my_engine some_engine_id some_brand some_weight")
interpret("create Car my_car some_id some_name some_color some_engine_id")
for clazz in relaxClasses:
    print(clazz.to_string())

    