import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ValueViewComponent } from './value-view.component';

describe('ValueViewComponent', () => {
  let component: ValueViewComponent;
  let fixture: ComponentFixture<ValueViewComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ValueViewComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ValueViewComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
