Rails.application.routes.draw do
  get 'home/index'
  root 'home#index'
 
  get 'home/gradeavgs', to: 'home#gradeavgs'  
  get 'home/amountofstudentsperclass', to: 'home#amountofstudentsperclass'    
  get 'home/completedexcersises/:percentage', to: 'home#completedexcersises'
  get 'home/kmeans/:percentage', to: 'home#kmeans'      
  get 'home/weekdaycompletionsvsgradesclassification/:percentage', to: 'home#weekdaycompletionsvsgradesclassification'  
  get 'home/weekendcompletionsvsgradesclassification/:percentage', to: 'home#weekendcompletionsvsgradesclassification'
  get 'home/daycompletionsvsgradesclassification/:percentage', to: 'home#daycompletionsvsgradesclassification'                             
  get 'home/nightcompletionsvsgradesclassification/:percentage', to: 'home#nightcompletionsvsgradesclassification'                
  get 'home/successrate/:percentage', to: 'home#successrate'
  get 'home/attemptsvsfailures', to: 'home#attemptsvsfailures'    
  get 'home/linearregression/:xyvalues', to: 'home#linearregression'
  get 'home/logregression/:xyvalues', to: 'home#logregression'  
  get 'home/correlation/:xyvalues', to: 'home#correlation'  
  get 'home/filteroutliers/:xyvalues', to: 'home#filteroutliers'  
  
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
 