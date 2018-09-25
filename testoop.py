class Person(object):

    def __init__(self, first, last):
        self.firstname = first
        self.lastname = last

    def toString(self):
        return self.firstname + " " + self.lastname

class Employee(Person,object):

    def __init__(self, first, last, staffnum):
        Person.__init__(self,first, last)
        self.staffnumber = staffnum

    def toString(self):
        return super(Employee, self).toString() + ", " +  self.staffnumber

x = Person("Marge", "Simpson")
y = Employee("Homer", "Simpson", "1007")

print(x.toString())
print(y.toString())
