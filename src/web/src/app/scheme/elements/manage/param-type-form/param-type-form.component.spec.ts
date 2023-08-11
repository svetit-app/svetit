import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ParamTypeFormComponent } from './param-type-form.component';

describe('ParamTypeFormComponent', () => {
  let component: ParamTypeFormComponent;
  let fixture: ComponentFixture<ParamTypeFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ParamTypeFormComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ParamTypeFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
