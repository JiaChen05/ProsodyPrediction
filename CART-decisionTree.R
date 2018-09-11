#CART决策树使用Gini指数
install.packages("tree")
library(tree)  
library(ISLR)

attach(Carseats) 
High=ifelse(Sales<=8,"No","Yes") #high的值yes or no 取决于Sales的大小
Carseats=data.frame(Carseats,High)#增加一列high 
View(Carseats)
fix(Carseats)  
set.seed(1)
train = sample(1:nrow(Carseats),200) #随机抽取200行数据作为训练集
Carseats.test = Carseats[-train,]#测试集
High.test = High[-train]
tree.carseats = tree(High~.-Sales,Carseats,subset=train) # 建立决策树模型，因变量high,自变量除High和Sales之外其他变量
dim(Carseats) 
summary(tree.carseats) 
plot(tree.carseats)#决策树可视化
text(tree.carseats,pretty = 0)
tree.pred = predict(tree.carseats,Carseats.test,type="class")#决策树预测
mat <- as.matrix(table(tree.pred,High.test))
mat
sum(diag(mat))/sum(mat)#预测正确率0.77
set.seed(2)
cv.carseats = cv.tree(tree.carseats,FUN = prune.misclass) # 以分类错误率为指标来剪枝
cv.carseats$size
cv.carseats$dev
plot(cv.carseats$size,cv.carseats$dev,type = "b") # 16个叶结点的决策树交叉验证误差最低
prune.carseats = prune.misclass(tree.carseats,best=16)#剪枝
plot(prune.carseats)#剪枝后的决策树可视化
text(prune.carseats,pretty = 0)
tree.pred = predict(prune.carseats,Carseats.test,type = "class")
mat <- as.matrix(table(tree.pred,High.test))
mat
sum(diag(mat))/sum(mat)#预测正确率0.77
